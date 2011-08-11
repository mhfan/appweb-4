/*
    alias.tst - ESP MVC tests for an EjsAlias
 */

const HTTP = (global.tsession && tsession["http"]) || ":4100"
let http: Http = new Http

let app = "/app2"
//  /app2
http.get(HTTP + app)
assert(http.status == 200)
assert(http.response.contains("Welcome to ESP"))
http.close()

//  /app2/
http.get(HTTP + app + "/")
assert(http.status == 200)
assert(http.response.contains("Welcome to ESP"))
http.close()

//  /app2/static/layout.css
http.get(HTTP + app + "/static/layout.css")
assert(http.status == 200)
assert(http.response.contains("Default layout style sheet"))
http.close()

//  /app2/static/index.esp
http.get(HTTP + app + "/static/index.esp")
assert(http.status == 200)
assert(http.response.contains("Welcome to ESP"))
http.close()

//  /app2/user/check - this tests a controller without view
http.get(HTTP + app + "/user/check")
assert(http.status == 200)
assert(http.response.contains("Check: OK"))

//  /app2/user/details - this tests templates
http.get(HTTP + app + "/user/details")
assert(http.status == 200)
assert(http.response.contains("<title>MVC Title</title>"))
assert(http.response.contains("Powered by Appweb"))
assert(http.response.contains("SECRET: 42"))
http.close()
