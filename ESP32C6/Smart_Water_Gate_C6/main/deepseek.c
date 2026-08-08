#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "esp_log.h"
#include "cJSON.h"
#include "deepseek.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/err.h"

/* Local ASP.NET proxy: no TLS, direct socket, minimal memory. */
#define PROXY_HOST CONFIG_WATER_WEB_SERVER_HOST
#define PROXY_PORT CONFIG_WATER_WEB_SERVER_PORT

esp_err_t deepseek_query(const char *user_message, char *response, size_t response_size)
{
    if (!user_message || !response || response_size == 0) return ESP_ERR_INVALID_ARG;
    response[0] = '\0';

    /* Build URL-encoded query */
    char q[512];
    int pos = 0;
    for (const char *p = user_message; *p && pos < (int)sizeof(q) - 4; p++) {
        unsigned char c = (unsigned char)*p;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')
            || c == '-' || c == '_' || c == '.' || c == '~') {
            q[pos++] = c;
        } else if (c == ' ') {
            q[pos++] = '+';
        } else {
            pos += snprintf(q + pos, sizeof(q) - pos, "%%%02X", c);
        }
    }
    q[pos] = '\0';

    printf("[DS] Resolving %s...\n", PROXY_HOST);
    struct hostent *he = gethostbyname(PROXY_HOST);
    if (!he) { printf("[DS] DNS fail\n"); return ESP_FAIL; }

    printf("[DS] Connecting...\n");
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { printf("[DS] Socket fail\n"); return ESP_FAIL; }

    struct sockaddr_in dest = {
        .sin_family = AF_INET,
        .sin_port = htons(PROXY_PORT),
    };
    memcpy(&dest.sin_addr, he->h_addr, he->h_length);

    struct timeval tv = { .tv_sec = 10, .tv_usec = 0 };
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    if (connect(sock, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        printf("[DS] Connect fail\n");
        close(sock);
        return ESP_FAIL;
    }

    char req[768];
    int len = snprintf(req, sizeof(req),
        "GET /api/ask?q=%s HTTP/1.0\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n", q, PROXY_HOST);

    printf("[DS] Sending...\n");
    if (write(sock, req, len) != len) {
        close(sock);
        return ESP_FAIL;
    }

    /* Read response */
    char buf[1280];
    int total = 0;
    int n;
    while ((n = read(sock, buf + total, sizeof(buf) - total - 1)) > 0) {
        total += n;
        if (total >= (int)sizeof(buf) - 1) break;
    }
    close(sock);
    buf[total] = '\0';

    printf("[DS] Got %d bytes\n", total);

    /* Find JSON body after HTTP headers */
    char *body = strstr(buf, "\r\n\r\n");
    if (!body) body = strstr(buf, "\n\n");
    if (!body) { printf("[DS] No body\n"); return ESP_FAIL; }
    body = strstr(body, "{");
    if (!body) { printf("[DS] No JSON\n"); return ESP_FAIL; }

    /* Parse {"ok":true,"reply":"..."} */
    cJSON *jresp = cJSON_Parse(body);
    if (!jresp) { printf("[DS] JSON parse fail\n"); return ESP_FAIL; }

    cJSON *jreply = cJSON_GetObjectItem(jresp, "reply");
    esp_err_t err = ESP_FAIL;
    if (cJSON_IsString(jreply) && jreply->valuestring) {
        strncpy(response, jreply->valuestring, response_size - 1);
        response[response_size - 1] = '\0';
        printf("[DS] Reply: %.80s\n", response);
        err = ESP_OK;
    }
    cJSON_Delete(jresp);

    if (response[0] == '\0') {
        snprintf(response, response_size, "error");
        return ESP_FAIL;
    }
    return err;
}
