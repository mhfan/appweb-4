/*
    Test caching
 */
#include "esp.h"

//  This is configured for caching by API below
static void api() {
    render("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
}

static void small() {
    int     i;
    //  This will emit ~8K (under the item limit)
    for (i = 0; i < 100; i++) {
        render("Line: %05d %s", i, "aaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbccccccccccccccccccddddddd<br/>\r\n");
    }
    render("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
}

static void big() {
    int     i;
    //  This will emit ~76K (under the item limit)
    for (i = 0; i < 1000; i++) {
        render("Line: %05d %s", i, "aaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbccccccccccccccccccddddddd<br/>\r\n");
    }
    // render("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
}

static void huge() { 
    int     i;
    //  This will emit ~762K (over the item limit)
    for (i = 0; i < 10000; i++) {
        render("Line: %05d %s", i, "aaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbccccccccccccccccccddddddd<br/>\r\n");
    }
    render("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
}

static void clear() { 
    espUpdateCache(getConn(), "/cache/manual", 0, 0);
    espUpdateCache(getConn(), "/cache/big", 0, 0);
    espUpdateCache(getConn(), "/cache/small", 0, 0);
    espUpdateCache(getConn(), "/cache/api", 0, 0);
    render("done");
}

static void client() { 
    render("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
}

static void manual() { 
    if (smatch(getQuery(), "send")) {
        setHeader("X-SendCache", "true");
        finalize();
    } else if (!espRenderCached(getConn())) {
        render("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
    }
}

static void update() { 
    cchar   *data;
    data = sfmt("{ when: %Ld, uri: '%s', query: '%s' }\r\n", mprGetTicks(), getUri(), getQuery());
    espUpdateCache(getConn(), "/cache/manual", data, 86400);
    render("done");
}

ESP_EXPORT int esp_controller_cache(EspRoute *eroute, MprModule *module) {
    HttpRoute *route;
    espDefineAction(eroute, "cache-cmd-api", api);
    espDefineAction(eroute, "cache-cmd-big", big);
    espDefineAction(eroute, "cache-cmd-small", small);
    espDefineAction(eroute, "cache-cmd-clear", clear);
    espDefineAction(eroute, "cache-cmd-client", client);
    espDefineAction(eroute, "cache-cmd-huge", huge);
    espDefineAction(eroute, "cache-cmd-manual", manual);
    espDefineAction(eroute, "cache-cmd-update", update);

    //  This will cache the next request after the first one that triggered the loading of this controller
    route = httpLookupRoute(eroute->route->host, "/app/*/default");
    espCache(route, "/cache/api", 0, 0);
    return 0;
}