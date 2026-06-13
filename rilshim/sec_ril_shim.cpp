#define LOG_TAG "SecRilShim"

#include <ctype.h>
#include <cutils/properties.h>
#include <dlfcn.h>
#include <log/log.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <telephony/ril.h>

#include <map>

static const char kStockRilPath[] = "/vendor/lib/libsec-ril.so";
static const int kSamsungRequestEnableUnsolResponse = 10034;

static const RIL_Env* g_real_env = NULL;
static const RIL_RadioFunctions* g_stock_functions = NULL;
static RIL_RadioFunctions g_shim_functions;
static pthread_mutex_t g_request_lock = PTHREAD_MUTEX_INITIALIZER;
static std::map<RIL_Token, int> g_requests;
static int g_enable_unsol_response_token;
static bool g_enable_unsol_response_sent = false;
static pthread_mutex_t g_operator_lock = PTHREAD_MUTEX_INITIALIZER;
static char g_operator_long[PROPERTY_VALUE_MAX];
static char g_operator_short[PROPERTY_VALUE_MAX];
static char g_operator_numeric[PROPERTY_VALUE_MAX];

static RIL_Token enable_unsol_response_token() {
    return static_cast<RIL_Token>(&g_enable_unsol_response_token);
}

static bool property_equals(const char* key, const char* expected) {
    char value[PROPERTY_VALUE_MAX] = {};
    property_get(key, value, "");
    return strcmp(value, expected) == 0;
}

static bool property_int_gt_zero(const char* key) {
    char value[PROPERTY_VALUE_MAX] = {};
    property_get(key, value, "");
    return atoi(value) > 0;
}

static bool sim_is_present() {
    return property_equals("ril.hasisim", "1") || property_int_gt_zero("ril.ICC_TYPE");
}

static void remember_request(RIL_Token token, int request) {
    pthread_mutex_lock(&g_request_lock);
    g_requests[token] = request;
    pthread_mutex_unlock(&g_request_lock);
}

static int take_request(RIL_Token token) {
    int request = -1;

    pthread_mutex_lock(&g_request_lock);
    std::map<RIL_Token, int>::iterator it = g_requests.find(token);
    if (it != g_requests.end()) {
        request = it->second;
        g_requests.erase(it);
    }
    pthread_mutex_unlock(&g_request_lock);

    return request;
}

static bool valid_app_index(int index, int app_count) {
    return index < 0 || index < app_count;
}

static void sanitize_card_status(RIL_CardStatus_v6* status) {
    if (status->num_applications < 0 || status->num_applications > RIL_CARD_MAX_APPS) {
        status->num_applications = 0;
    }

    if (!valid_app_index(status->gsm_umts_subscription_app_index, status->num_applications)) {
        status->gsm_umts_subscription_app_index = -1;
    }
    if (!valid_app_index(status->cdma_subscription_app_index, status->num_applications)) {
        status->cdma_subscription_app_index = -1;
    }
    if (!valid_app_index(status->ims_subscription_app_index, status->num_applications)) {
        status->ims_subscription_app_index = -1;
    }

    if (status->card_state == RIL_CARDSTATE_PRESENT &&
        status->num_applications > 0 &&
        status->gsm_umts_subscription_app_index < 0) {
        status->gsm_umts_subscription_app_index = 0;
    }
}

static bool card_status_is_valid(const RIL_CardStatus_v6* status) {
    if (status == NULL ||
        status->num_applications < 0 ||
        status->num_applications > RIL_CARD_MAX_APPS) {
        return false;
    }

    return valid_app_index(status->gsm_umts_subscription_app_index, status->num_applications) &&
           valid_app_index(status->cdma_subscription_app_index, status->num_applications) &&
           valid_app_index(status->ims_subscription_app_index, status->num_applications);
}

static void fill_fallback_card_status(RIL_CardStatus_v6* status) {
    memset(status, 0, sizeof(*status));

    status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
    status->gsm_umts_subscription_app_index = -1;
    status->cdma_subscription_app_index = -1;
    status->ims_subscription_app_index = -1;

    if (!sim_is_present()) {
        status->card_state = RIL_CARDSTATE_ABSENT;
        status->num_applications = 0;
        return;
    }

    status->card_state = RIL_CARDSTATE_PRESENT;
    status->universal_pin_state = RIL_PINSTATE_DISABLED;
    status->gsm_umts_subscription_app_index = 0;
    status->num_applications = 1;

    RIL_AppStatus* app = &status->applications[0];
    app->app_type = property_equals("ril.ICC_TYPE", "1") ? RIL_APPTYPE_SIM : RIL_APPTYPE_USIM;
    app->app_state = RIL_APPSTATE_READY;
    app->perso_substate = RIL_PERSOSUBSTATE_READY;
    app->aid_ptr = NULL;
    app->app_label_ptr = const_cast<char*>("USIM");
    app->pin1_replaced = 0;
    app->pin1 = RIL_PINSTATE_DISABLED;
    app->pin2 = RIL_PINSTATE_UNKNOWN;
}

static bool make_card_status(void* response, size_t response_len, RIL_CardStatus_v6* fixed) {
    if (response != NULL && response_len == sizeof(RIL_CardStatus_v6)) {
        memcpy(fixed, response, sizeof(*fixed));
        sanitize_card_status(fixed);
        if (card_status_is_valid(fixed) &&
            !(fixed->card_state == RIL_CARDSTATE_PRESENT && fixed->num_applications == 0)) {
            return true;
        }
    }

    if (response != NULL && response_len == sizeof(RIL_CardStatus_v5)) {
        const RIL_CardStatus_v5* old_status = static_cast<const RIL_CardStatus_v5*>(response);
        memset(fixed, 0, sizeof(*fixed));
        fixed->card_state = old_status->card_state;
        fixed->universal_pin_state = old_status->universal_pin_state;
        fixed->gsm_umts_subscription_app_index = old_status->gsm_umts_subscription_app_index;
        fixed->cdma_subscription_app_index = old_status->cdma_subscription_app_index;
        fixed->ims_subscription_app_index = -1;
        fixed->num_applications = old_status->num_applications;
        memcpy(fixed->applications, old_status->applications, sizeof(old_status->applications));
        sanitize_card_status(fixed);
        if (card_status_is_valid(fixed)) {
            ALOGW("Converted v5 SIM status response to v6");
            return true;
        }
    }

    fill_fallback_card_status(fixed);
    ALOGW("Synthesized SIM status response for invalid stock RIL payload");
    return true;
}

static void property_or_default(const char* key, const char* fallback, char* out, size_t out_len) {
    property_get(key, out, fallback);
    if (out[0] == '\0') {
        strlcpy(out, fallback, out_len);
    }
}

static bool response_is_string_array(void* response, size_t response_len, size_t count) {
    return response != NULL && response_len == count * sizeof(char*);
}

static bool response_has_string_array(void* response, size_t response_len, size_t count) {
    return response != NULL && response_len >= count * sizeof(char*);
}

static bool is_valid_plmn(const char* value) {
    if (value == NULL) {
        return false;
    }

    size_t len = strlen(value);
    if (len != 5 && len != 6) {
        return false;
    }

    for (size_t i = 0; i < len; ++i) {
        if (!isdigit(static_cast<unsigned char>(value[i]))) {
            return false;
        }
    }

    return true;
}

static bool copy_property_plmn(const char* key, char* out, size_t out_len) {
    char value[PROPERTY_VALUE_MAX] = {};
    property_get(key, value, "");
    if (!is_valid_plmn(value)) {
        return false;
    }

    strlcpy(out, value, out_len);
    return true;
}

static bool copy_cached_operator(char* long_name,
                                 size_t long_name_len,
                                 char* short_name,
                                 size_t short_name_len,
                                 char* numeric,
                                 size_t numeric_len) {
    bool valid;

    pthread_mutex_lock(&g_operator_lock);
    valid = is_valid_plmn(g_operator_numeric);
    if (valid) {
        strlcpy(long_name, g_operator_long, long_name_len);
        strlcpy(short_name, g_operator_short, short_name_len);
        strlcpy(numeric, g_operator_numeric, numeric_len);
    }
    pthread_mutex_unlock(&g_operator_lock);

    return valid;
}

static void remember_operator(const char* long_name,
                              const char* short_name,
                              const char* numeric) {
    if (!is_valid_plmn(numeric)) {
        return;
    }

    char fixed_long[PROPERTY_VALUE_MAX] = {};
    char fixed_short[PROPERTY_VALUE_MAX] = {};
    strlcpy(fixed_long,
            long_name != NULL && long_name[0] != '\0' ? long_name : numeric,
            sizeof(fixed_long));
    strlcpy(fixed_short,
            short_name != NULL && short_name[0] != '\0' ? short_name : fixed_long,
            sizeof(fixed_short));

    bool changed;
    pthread_mutex_lock(&g_operator_lock);
    changed = strcmp(g_operator_numeric, numeric) != 0;
    strlcpy(g_operator_long, fixed_long, sizeof(g_operator_long));
    strlcpy(g_operator_short, fixed_short, sizeof(g_operator_short));
    strlcpy(g_operator_numeric, numeric, sizeof(g_operator_numeric));
    pthread_mutex_unlock(&g_operator_lock);

    property_set("ril.operator.alpha", fixed_long);
    property_set("ril.operator.numeric", numeric);

    if (changed && sim_is_present() && g_real_env != NULL) {
        ALOGI("Cached operator %s/%s and requested SIM records refresh",
              fixed_long, numeric);
#if defined(ANDROID_MULTI_SIM)
        g_real_env->OnUnsolicitedResponse(RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED,
                                          NULL, 0, RIL_SOCKET_1);
#else
        g_real_env->OnUnsolicitedResponse(RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED,
                                          NULL, 0);
#endif
    }
}

static bool is_sms_request(int request) {
    return request == RIL_REQUEST_SEND_SMS ||
           request == RIL_REQUEST_SEND_SMS_EXPECT_MORE ||
           request == RIL_REQUEST_IMS_SEND_SMS;
}

static bool send_normalized_sms_response(RIL_Token token, void* response, size_t response_len) {
    if (response != NULL && response_len == sizeof(RIL_SMS_Response)) {
        RIL_SMS_Response* sms = static_cast<RIL_SMS_Response*>(response);
        if (sms->messageRef > 0 && sms->ackPDU != NULL) {
            return false;
        }

        RIL_SMS_Response fixed = *sms;
        if (fixed.messageRef <= 0) {
            fixed.messageRef = 1;
        }
        if (fixed.ackPDU == NULL) {
            fixed.ackPDU = const_cast<char*>("");
        }
        if (fixed.errorCode == 0) {
            fixed.errorCode = -1;
        }

        ALOGW("Normalized SMS response fields for stock RIL payload");
        g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, &fixed, sizeof(fixed));
        return true;
    }

    RIL_SMS_Response fixed = {};
    fixed.messageRef = 1;
    fixed.ackPDU = const_cast<char*>("");
    fixed.errorCode = -1;

    ALOGW("Synthesized SMS response for invalid stock RIL payload");
    g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, &fixed, sizeof(fixed));
    return true;
}

static bool is_samsung_oem_unsol(int unsol_response) {
    /* Samsung framework extensions use these OEM unsolicited IDs. AOSP RILC
     * rejects them, so drop only the IDs observed from the stock RIL. */
    switch (unsol_response) {
        case 11008:
        case 11010:
        case 11021:
        case 11024:
        case 11066:
        case 20017:
            return true;
        default:
            return false;
    }
}

static bool complete_request_in_shim(int request, RIL_Token token) {
    /* Keep Oreo framework bookkeeping requests away from the Marshmallow-era
     * stock RIL when they are unsupported or return malformed payloads. */
    switch (request) {
#if defined(RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE)
        case RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE:
#endif
#if defined(RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION)
        case RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION:
#endif
#if defined(RIL_REQUEST_SEND_DEVICE_STATE)
        case RIL_REQUEST_SEND_DEVICE_STATE:
#endif
#if defined(RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER)
        case RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER:
#endif
            g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, NULL, 0);
            return true;

#if defined(RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE)
        case RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE: {
            int response[1] = { 0 };
            g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, response, sizeof(response));
            return true;
        }
#endif

#if defined(RIL_REQUEST_START_LCE)
        case RIL_REQUEST_START_LCE:
#endif
#if defined(RIL_REQUEST_STOP_LCE)
        case RIL_REQUEST_STOP_LCE:
#endif
#if defined(RIL_REQUEST_PULL_LCEDATA)
        case RIL_REQUEST_PULL_LCEDATA:
#endif
        {
            int response[2] = { 0, 0 };
            g_real_env->OnRequestComplete(token, RIL_E_REQUEST_NOT_SUPPORTED,
                                          response, sizeof(response));
            return true;
        }

#if defined(RIL_REQUEST_GET_HARDWARE_CONFIG)
        case RIL_REQUEST_GET_HARDWARE_CONFIG:
            g_real_env->OnRequestComplete(token, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            return true;
#endif
        default:
            return false;
    }
}

static void send_ims_registration_state(RIL_Token token) {
    int response[2] = { 0, RADIO_TECH_3GPP };
    ALOGW("Synthesized IMS registration state for invalid stock RIL payload");
    g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, response, sizeof(response));
}

static size_t response_string_count(void* response, size_t response_len) {
    if (response == NULL || response_len < sizeof(char*) ||
        response_len % sizeof(char*) != 0) {
        return 0;
    }

    return response_len / sizeof(char*);
}

static const char* stock_string_at(char** stock,
                                   size_t stock_count,
                                   size_t index,
                                   const char* fallback) {
    if (index < stock_count && stock[index] != NULL && stock[index][0] != '\0') {
        return stock[index];
    }

    return fallback;
}

static bool is_registered_state(const char* reg_state) {
    return strcmp(reg_state, "1") == 0 || strcmp(reg_state, "5") == 0;
}

static void send_voice_registration(RIL_Token token,
                                    void* response,
                                    size_t response_len) {
    char** stock = static_cast<char**>(response);
    size_t stock_count = response_string_count(response, response_len);
    char rat[PROPERTY_VALUE_MAX] = {};
    property_or_default("ril.voice.rat", "3", rat, sizeof(rat));

    const char* reg_state = stock_string_at(stock, stock_count, 0, "0");
    if (!is_registered_state(reg_state) && stock_count <= 3) {
        strlcpy(rat, "0", sizeof(rat));
    }

    char* fixed[15] = {
        const_cast<char*>(reg_state),
        const_cast<char*>(stock_string_at(stock, stock_count, 1, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 2, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 3, rat)),
        const_cast<char*>(stock_string_at(stock, stock_count, 4, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 5, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 6, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 7, "0")),
        const_cast<char*>(stock_string_at(stock, stock_count, 8, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 9, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 10, "-1")),
        const_cast<char*>(stock_string_at(stock, stock_count, 11, "-1")),
        const_cast<char*>(stock_string_at(stock, stock_count, 12, "-1")),
        const_cast<char*>(stock_string_at(stock, stock_count, 13, "0")),
        const_cast<char*>(stock_string_at(stock, stock_count, 14, ""))
    };

    ALOGW("Normalized voice registration payload len=%zu count=%zu state=%s rat=%s",
          response_len, stock_count, fixed[0], fixed[3]);
    g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, fixed, sizeof(fixed));
}

static void send_data_registration(RIL_Token token,
                                   void* response,
                                   size_t response_len) {
    char** stock = static_cast<char**>(response);
    size_t stock_count = response_string_count(response, response_len);
    char rat[PROPERTY_VALUE_MAX] = {};
    property_get("ril.data.rat", rat, "");
    if (rat[0] == '\0') {
        property_get("ril.voice.rat", rat, "");
    }
    if (rat[0] == '\0') {
        strlcpy(rat, "0", sizeof(rat));
    }

    const char* reg_state = stock_string_at(stock, stock_count, 0, "0");
    if (!is_registered_state(reg_state)) {
        strlcpy(rat, "0", sizeof(rat));
    }

    char* fixed[6] = {
        const_cast<char*>(reg_state),
        const_cast<char*>(stock_string_at(stock, stock_count, 1, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 2, "")),
        const_cast<char*>(stock_string_at(stock, stock_count, 3, rat)),
        const_cast<char*>(stock_string_at(stock, stock_count, 4, "0")),
        const_cast<char*>(stock_string_at(stock, stock_count, 5, "1"))
    };

    ALOGW("Normalized data registration payload len=%zu count=%zu state=%s rat=%s",
          response_len, stock_count, fixed[0], fixed[3]);
    g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, fixed, sizeof(fixed));
}

static void fill_operator(char* long_name,
                          size_t long_name_len,
                          char* short_name,
                          size_t short_name_len,
                          char* numeric,
                          size_t numeric_len) {
    if (!copy_property_plmn("ril.operator.numeric", numeric, numeric_len) &&
        !copy_cached_operator(long_name, long_name_len, short_name, short_name_len,
                              numeric, numeric_len) &&
        !copy_property_plmn("gsm.operator.numeric", numeric, numeric_len) &&
        !copy_property_plmn("gsm.sim.operator.numeric", numeric, numeric_len)) {
        numeric[0] = '\0';
    }

    if (long_name[0] == '\0') {
        property_get("gsm.operator.alpha", long_name, "");
    }
    if (long_name[0] == '\0') {
        property_get("ril.operator.alpha", long_name, "");
    }
    if (long_name[0] == '\0') {
        property_get("persist.radio.plmnname", long_name, "");
    }
    if (long_name[0] == '\0' && numeric[0] != '\0') {
        strlcpy(long_name, numeric, long_name_len);
    }

    strlcpy(short_name, long_name, short_name_len);
}

static void send_operator(RIL_Token token, const char* reason) {
    char long_name[PROPERTY_VALUE_MAX] = {};
    char short_name[PROPERTY_VALUE_MAX] = {};
    char numeric[PROPERTY_VALUE_MAX] = {};
    fill_operator(long_name, sizeof(long_name), short_name, sizeof(short_name),
                  numeric, sizeof(numeric));

    char* response[3] = { long_name, short_name, numeric };
    if (is_valid_plmn(numeric)) {
        remember_operator(long_name, short_name, numeric);
    }
    ALOGW("Synthesized operator response for %s stock RIL payload: %s/%s",
          reason, long_name, numeric);
    g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, response, sizeof(response));
}

static bool send_normalized_operator(RIL_Token token, void* response,
                                     size_t response_len) {
    if (response_has_string_array(response, response_len, 3)) {
        char** stock = static_cast<char**>(response);
        const char* long_name = stock[0] != NULL ? stock[0] : "";
        const char* short_name = stock[1] != NULL ? stock[1] : "";
        const char* numeric = stock[2] != NULL ? stock[2] : "";

        if (is_valid_plmn(numeric)) {
            char fixed_long[PROPERTY_VALUE_MAX] = {};
            char fixed_short[PROPERTY_VALUE_MAX] = {};
            char fixed_numeric[PROPERTY_VALUE_MAX] = {};
            strlcpy(fixed_long, long_name[0] != '\0' ? long_name : numeric,
                    sizeof(fixed_long));
            strlcpy(fixed_short, short_name[0] != '\0' ? short_name : fixed_long,
                    sizeof(fixed_short));
            strlcpy(fixed_numeric, numeric, sizeof(fixed_numeric));

            char* fixed[3] = { fixed_long, fixed_short, fixed_numeric };
            remember_operator(fixed_long, fixed_short, fixed_numeric);
            if (response_len != 3 * sizeof(char*) ||
                long_name[0] == '\0' ||
                short_name[0] == '\0') {
                ALOGW("Normalized operator payload len=%zu to %s/%s",
                      response_len, fixed_long, fixed_numeric);
            }
            g_real_env->OnRequestComplete(token, RIL_E_SUCCESS,
                                          fixed, sizeof(fixed));
            return true;
        }

        ALOGW("Rejected operator payload len=%zu numeric='%s'",
              response_len, numeric);
    }

    send_operator(token, response == NULL ? "missing" : "invalid");
    return true;
}

static bool build_fallback_imsi(char* imsi, size_t imsi_len) {
    if (!sim_is_present() || imsi_len < 16) {
        return false;
    }

    char long_name[PROPERTY_VALUE_MAX] = {};
    char short_name[PROPERTY_VALUE_MAX] = {};
    char numeric[PROPERTY_VALUE_MAX] = {};
    fill_operator(long_name, sizeof(long_name), short_name, sizeof(short_name),
                  numeric, sizeof(numeric));

    size_t numeric_len = strlen(numeric);
    if (numeric_len < 5 || numeric_len >= imsi_len) {
        return false;
    }

    /* This is only a framework bootstrap fallback. Never invent a carrier:
     * use a known MCC/MNC prefix or fail and let telephony retry later. */
    strlcpy(imsi, numeric, imsi_len);
    while (strlen(imsi) < 15) {
        size_t len = strlen(imsi);
        imsi[len] = '0';
        imsi[len + 1] = '\0';
    }

    return true;
}

static void send_imsi(RIL_Token token) {
    char imsi[16] = {};
    if (!build_fallback_imsi(imsi, sizeof(imsi))) {
        g_real_env->OnRequestComplete(token, RIL_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    ALOGW("Synthesized IMSI prefix for framework SIM records");
    g_real_env->OnRequestComplete(token, RIL_E_SUCCESS, imsi, strlen(imsi) + 1);
}

static int normalized_call_index(int index, int fallback) {
    if (index > 0 && index <= 16) {
        return index;
    }

    int low = index & 0xff;
    if (low > 0 && low <= 16) {
        return low;
    }

    return fallback;
}

static bool valid_call_state(RIL_CallState state) {
    int value = static_cast<int>(state);
    return value >= static_cast<int>(RIL_CALL_ACTIVE) &&
           value <= static_cast<int>(RIL_CALL_WAITING);
}

static RIL_CallState normalized_call_state(RIL_CallState state, int is_mt) {
    if (valid_call_state(state)) {
        return state;
    }

    int low = static_cast<int>(state) & 0xff;
    if (low >= static_cast<int>(RIL_CALL_ACTIVE) &&
        low <= static_cast<int>(RIL_CALL_WAITING)) {
        return static_cast<RIL_CallState>(low);
    }

    return is_mt ? RIL_CALL_INCOMING : RIL_CALL_ACTIVE;
}

static bool send_normalized_call_list(RIL_Token token, RIL_Errno error,
                                      void* response, size_t response_len) {
    if (response == NULL || response_len % sizeof(RIL_Call*) != 0) {
        return false;
    }

    size_t count = response_len / sizeof(RIL_Call*);
    if (count == 0 || count > 16) {
        return false;
    }

    RIL_Call fixed_calls[16];
    RIL_Call* fixed_ptrs[16];
    RIL_Call** calls = static_cast<RIL_Call**>(response);
    bool changed = false;

    for (size_t i = 0; i < count; ++i) {
        if (calls[i] == NULL) {
            return false;
        }

        fixed_calls[i] = *calls[i];
        int fixed_index = normalized_call_index(fixed_calls[i].index, static_cast<int>(i + 1));
        if (fixed_index != fixed_calls[i].index) {
            ALOGW("Normalized Samsung call index %d to %d",
                  fixed_calls[i].index, fixed_index);
            fixed_calls[i].index = fixed_index;
            changed = true;
        }

        RIL_CallState fixed_state =
                normalized_call_state(fixed_calls[i].state, fixed_calls[i].isMT);
        if (fixed_state != fixed_calls[i].state) {
            ALOGW("Normalized Samsung call state %d to %d",
                  static_cast<int>(fixed_calls[i].state),
                  static_cast<int>(fixed_state));
            fixed_calls[i].state = fixed_state;
            changed = true;
        }

        fixed_ptrs[i] = &fixed_calls[i];
    }

    if (!changed) {
        return false;
    }

    g_real_env->OnRequestComplete(token, error, fixed_ptrs, count * sizeof(RIL_Call*));
    return true;
}

static void on_request_complete(RIL_Token token, RIL_Errno error,
                                void* response, size_t response_len) {
    if (token == enable_unsol_response_token()) {
        ALOGI("Samsung enable-unsol-response request completed with error %d", error);
        return;
    }

    int request = take_request(token);

    if (error == RIL_E_SUCCESS) {
        if (request == RIL_REQUEST_GET_CURRENT_CALLS &&
                   send_normalized_call_list(token, error, response, response_len)) {
            return;
        } else if (request == RIL_REQUEST_GET_SIM_STATUS) {
            RIL_CardStatus_v6 fixed;
            if (make_card_status(response, response_len, &fixed)) {
                g_real_env->OnRequestComplete(token, error, &fixed, sizeof(fixed));
                return;
            }
        } else if (request == RIL_REQUEST_GET_IMSI &&
                   (response == NULL || static_cast<char*>(response)[0] == '\0')) {
            send_imsi(token);
            return;
        } else if (is_sms_request(request) &&
                   send_normalized_sms_response(token, response, response_len)) {
            return;
        } else if (request == RIL_REQUEST_IMS_REGISTRATION_STATE &&
                   (response == NULL || response_len != 2 * sizeof(int))) {
            send_ims_registration_state(token);
            return;
        } else if (request == RIL_REQUEST_VOICE_REGISTRATION_STATE &&
                   !response_is_string_array(response, response_len, 15)) {
            send_voice_registration(token, response, response_len);
            return;
        } else if (request == RIL_REQUEST_DATA_REGISTRATION_STATE &&
                   !response_is_string_array(response, response_len, 6) &&
                   !response_is_string_array(response, response_len, 11)) {
            send_data_registration(token, response, response_len);
            return;
        } else if (request == RIL_REQUEST_OPERATOR &&
                   send_normalized_operator(token, response, response_len)) {
            return;
        }
    } else if (request == RIL_REQUEST_GET_IMSI && sim_is_present()) {
        send_imsi(token);
        return;
    }

    g_real_env->OnRequestComplete(token, error, response, response_len);
}

#if defined(ANDROID_MULTI_SIM)
static void on_unsolicited_response(int unsol_response, const void* data,
                                    size_t data_len, RIL_SOCKET_ID socket_id) {
    if (is_samsung_oem_unsol(unsol_response)) {
        ALOGV("Dropped Samsung OEM unsolicited response %d", unsol_response);
        return;
    }

    g_real_env->OnUnsolicitedResponse(unsol_response, data, data_len, socket_id);
}
#else
static void on_unsolicited_response(int unsol_response, const void* data, size_t data_len) {
    if (is_samsung_oem_unsol(unsol_response)) {
        ALOGV("Dropped Samsung OEM unsolicited response %d", unsol_response);
        return;
    }

    g_real_env->OnUnsolicitedResponse(unsol_response, data, data_len);
}
#endif

static void request_timed_callback(RIL_TimedCallback callback,
                                   void* param,
                                   const struct timeval* relative_time) {
    g_real_env->RequestTimedCallback(callback, param, relative_time);
}

static void on_request_ack(RIL_Token token) {
    if (g_real_env->OnRequestAck != NULL) {
        g_real_env->OnRequestAck(token);
    }
}

static void enable_samsung_unsol_response(void*) {
    if (g_stock_functions == NULL || g_stock_functions->onRequest == NULL ||
        g_enable_unsol_response_sent) {
        return;
    }

    g_enable_unsol_response_sent = true;
    ALOGI("Sending Samsung enable-unsol-response request");

#if defined(ANDROID_MULTI_SIM)
    g_stock_functions->onRequest(kSamsungRequestEnableUnsolResponse, NULL, 0,
                                 enable_unsol_response_token(), RIL_SOCKET_1);
#else
    g_stock_functions->onRequest(kSamsungRequestEnableUnsolResponse, NULL, 0,
                                 enable_unsol_response_token());
#endif
}

#if defined(ANDROID_MULTI_SIM)
static void on_request(int request, void* data, size_t data_len,
                       RIL_Token token, RIL_SOCKET_ID socket_id) {
    if (complete_request_in_shim(request, token)) {
        return;
    }

    remember_request(token, request);
    g_stock_functions->onRequest(request, data, data_len, token, socket_id);
}
#else
static void on_request(int request, void* data, size_t data_len, RIL_Token token) {
    if (complete_request_in_shim(request, token)) {
        return;
    }

    remember_request(token, request);
    g_stock_functions->onRequest(request, data, data_len, token);
}
#endif

#if defined(ANDROID_MULTI_SIM)
static RIL_RadioState on_state_request(RIL_SOCKET_ID socket_id) {
    return g_stock_functions->onStateRequest(socket_id);
}
#else
static RIL_RadioState on_state_request() {
    return g_stock_functions->onStateRequest();
}
#endif

static int supports(int request_code) {
    return g_stock_functions->supports != NULL ? g_stock_functions->supports(request_code) : 1;
}

static void on_cancel(RIL_Token token) {
    if (g_stock_functions->onCancel != NULL) {
        g_stock_functions->onCancel(token);
    }
}

static const char* get_version() {
    if (g_stock_functions->getVersion != NULL) {
        return g_stock_functions->getVersion();
    }
    return "Samsung RIL shim";
}

extern "C" const RIL_RadioFunctions* RIL_Init(const RIL_Env* env, int argc, char** argv) {
    typedef const RIL_RadioFunctions* (*RilInit)(const RIL_Env*, int, char**);

    void* handle = dlopen(kStockRilPath, RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
        ALOGE("Failed to load stock RIL %s: %s", kStockRilPath, dlerror());
        return NULL;
    }

    RilInit stock_init = reinterpret_cast<RilInit>(dlsym(handle, "RIL_Init"));
    if (stock_init == NULL) {
        ALOGE("Stock RIL_Init not found: %s", dlerror());
        return NULL;
    }

    static RIL_Env shim_env;
    shim_env.OnRequestComplete = on_request_complete;
    shim_env.OnUnsolicitedResponse = on_unsolicited_response;
    shim_env.RequestTimedCallback = request_timed_callback;
    shim_env.OnRequestAck = on_request_ack;

    g_real_env = env;
    g_stock_functions = stock_init(&shim_env, argc, argv);
    if (g_stock_functions == NULL) {
        ALOGE("Stock RIL_Init returned NULL");
        return NULL;
    }

    memset(&g_shim_functions, 0, sizeof(g_shim_functions));
    g_shim_functions.version = g_stock_functions->version;
    g_shim_functions.onRequest = on_request;
    g_shim_functions.onStateRequest = on_state_request;
    g_shim_functions.supports = supports;
    g_shim_functions.onCancel = on_cancel;
    g_shim_functions.getVersion = get_version;

    ALOGI("Loaded Samsung RIL through compatibility shim, version %d",
          g_shim_functions.version);

    struct timeval enable_unsol_delay = { 0, 250000 };
    g_real_env->RequestTimedCallback(enable_samsung_unsol_response, NULL,
                                     &enable_unsol_delay);

    return &g_shim_functions;
}
