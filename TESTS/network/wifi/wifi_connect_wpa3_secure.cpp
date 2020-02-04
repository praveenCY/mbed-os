/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

#if defined(MBED_CONF_APP_WIFI_WPA3_SSID) && defined(MBED_CONF_APP_WIFI_WPA3_PASSWORD) &&\
    defined(MBED_CONF_APP_WIFI_WPA3_PROTOCOL)

nsapi_security get_wpa3_security()
{
#if defined MBED_CONF_APP_WIFI_WPA3_PROTOCOL
   static const char *SEC_WPA3 = "WPA3";
   static const char *SEC_WPA3_WPA2 = "WPA3/WPA2";

   if (strcmp(MBED_CONF_APP_WIFI_WPA3_PROTOCOL, SEC_WPA3) == 0) {
        return NSAPI_SECURITY_WPA3;
    }
   if (strcmp(MBED_CONF_APP_WIFI_WPA3_PROTOCOL, SEC_WPA3_WPA2) == 0) {
        return NSAPI_SECURITY_WPA3_WPA2;
    }
#endif
    return NSAPI_SECURITY_WPA3;
}

void wifi_connect_wpa3_secure(void)
{
    WiFiInterface *wifi = get_interface();

    // Driver shall cache the credentials
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(MBED_CONF_APP_WIFI_WPA3_SSID, MBED_CONF_APP_WIFI_WPA3_PASSWORD, get_wpa3_security()));
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->disconnect());

    /* Verify with wrong password */
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(MBED_CONF_APP_WIFI_WPA3_SSID, "aaaaaaaa", get_wpa3_security()));
    error = wifi->connect();
    wifi->disconnect();
    TEST_ASSERT(error == NSAPI_ERROR_AUTH_FAILURE ||
                error == NSAPI_ERROR_CONNECTION_TIMEOUT ||
                error == NSAPI_ERROR_NO_CONNECTION);

    /* Re-connect and make sure it is connecting properly */
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(MBED_CONF_APP_WIFI_WPA3_SSID, MBED_CONF_APP_WIFI_WPA3_PASSWORD, get_wpa3_security()));
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->disconnect());
}

#endif // defined(MBED_CONF_APP_WIFI_SECURE_SSID)&& defined(MBED_CONF_APP_WIFI_WPA3_PASSWORD) &&\
    defined(MBED_CONF_APP_WIFI_WPA3_PROTOCOL)
