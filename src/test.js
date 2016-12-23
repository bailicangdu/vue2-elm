webpackJsonp([23], {
    0: function(t, e, a) {
        t.exports = a(176)
    },
    113: function(t, e) {
        "use strict";
        Object.defineProperty(e, "__esModule", {
            value: !0
        });
        var a = function(t) {
            var e = document.createElement("a");
            e.href = t;
            var a = ["http:", "https:", "eleme:"];
            return a.indexOf(e.protocol) !== -1 && !(e.hostname && !e.hostname.match(/(^|\.)ele(net)?\.me$/))
        };
        e.default = {
            post: function(t, e) {
                var a = {
                    method: "POST",
                    credentials: "include",
                    body: JSON.stringify(e)
                };
                return window.fetch(t, a).then(function(t) {
                    var e = t.json();
                    return t.ok ? e : e.then(Promise.reject.bind(Promise))
                })
            },
            redirect: function() {
                var t = (new window.UParams).redirect;
                t && a(t) || (t = "/msite/"),
                location.href = t
            }
        }
    },
    140: function(t, e, a) {
        var s, o, i = {};
        a(559),
        s = a(329),
        o = a(878),
        t.exports = s || {},
        t.exports.__esModule && (t.exports = t.exports.default);
        var c = "function" == typeof t.exports ? t.exports.options || (t.exports.options = {}) : t.exports;
        o && (c.template = o),
        c.computed || (c.computed = {}),
        Object.keys(i).forEach(function(t) {
            var e = i[t];
            c.computed[t] = function() {
                return e
            }
        })
    },
    176: function(t, e, a) {
        "use strict";
        var s = a(1006)
          , o = babelHelpers.interopRequireDefault(s);
        new Vue({
            el: "body",
            components: {
                App: o.default
            }
        })
    },
    328: function(t, e, a) {
        "use strict";
        Object.defineProperty(e, "__esModule", {
            value: !0
        });
        var s = a(1007)
          , o = babelHelpers.interopRequireDefault(s)
          , i = a(1008)
          , c = babelHelpers.interopRequireDefault(i)
          , n = a(4);
        e.default = {
            components: {
                ElemeHeader: n.ElemeHeader,
                Message: o.default,
                Password: c.default
            },
            data: function() {
                return {
                    apihost: "//mainsite-restapi.ele.me",
                    current: "message",
                    toast: "",
                    timer: null,
                    showToast: !1,
                    isApp: /Eleme/.test(navigator.userAgent)
                }
            },
            computed: {
                headerOpt: function() {
                    return {
                        message: {
                            title: "登录",
                            switchName: "密码登录",
                            switchTarget: "password"
                        },
                        password: {
                            title: "密码登录",
                            switchName: "短信登录",
                            switchTarget: "message"
                        }
                    }[this.current]
                }
            },
            methods: {
                switchTo: function(t) {
                    this.current = t
                }
            },
            events: {
                setToast: function(t) {
                    var e = this;
                    clearTimeout(this.timer),
                    this.toast = t,
                    this.showToast = !0,
                    this.timer = setTimeout(function() {
                        e.showToast = !1
                    }, 2e3)
                }
            }
        }
    },
    329: function(t, e) {
        "use strict";
        Object.defineProperty(e, "__esModule", {
            value: !0
        }),
        e.default = {
            props: {
                apihost: {
                    type: String,
                    default: "//mainsite-restapi.ele.me"
                },
                confirmButton: {
                    type: Function,
                    default: function() {}
                },
                cancelButton: {
                    type: Function,
                    default: function() {}
                }
            },
            data: function() {
                return {
                    captchaCode: "",
                    userCaptcha: ""
                }
            },
            computed: {
                captchaImage: function() {
                    if (this.captchaCode)
                        return this.apihost + "/v1/captchas/" + this.captchaCode
                }
            },
            methods: {
                $fetch: function(t, e) {
                    return window.fetch(t, e).then(function(t) {
                        var e = t.json();
                        return t.status >= 200 && t.status < 300 ? e : e.then(Promise.reject.bind(Promise))
                    })
                },
                reloadCaptcha: function() {
                    this.$emit("getCaptcha")
                },
                submitCaptcha: function() {
                    return this.userCaptcha ? void this.confirmButton(this.userCaptcha, this.captchaCode) : this.$dispatch("setToast", "请填写验证码")
                }
            },
            events: {
                getCaptcha: function() {
                    var t = this;
                    this.userCaptcha = "",
                    this.$fetch(this.apihost + "/v1/captchas", {
                        method: "POST",
                        credentials: "include"
                    }).then(function(e) {
                        t.captchaCode = e.code
                    }).catch(function() {
                        return {}
                    })
                }
            }
        }
    },
    330: function(t, e, a) {
        "use strict";
        Object.defineProperty(e, "__esModule", {
            value: !0
        });
        var s = a(140)
          , o = babelHelpers.interopRequireDefault(s)
          , i = a(113)
          , c = babelHelpers.interopRequireDefault(i);
        e.default = {
            components: {
                Captcha: o.default
            },
            props: ["apihost", "current", "is-app"],
            data: function() {
                return {
                    isShowLayer: void 0,
                    interval: null,
                    countdown: 0,
                    mobilePhone: "",
                    msgCaptcha: "",
                    msgToken: ""
                }
            },
            computed: {
                isMobile: function t() {
                    var t = /(^(13\d|15[^4,\D]|17[13678]|18\d)\d{8}|170[^346,\D]\d{7})$/;
                    return t.test(this.mobilePhone)
                },
                enableShowLayer: function() {
                    return !this.isShowLayer && this.isMobile
                },
                buttonText: function() {
                    return "undefined" == typeof this.isShowLayer ? "获取验证码" : this.isShowLayer ? "发送中..." : "重新获取"
                }
            },
            methods: {
                showLayer: function() {
                    this.enableShowLayer && (this.$broadcast("getCaptcha"),
                    this.isShowLayer = !0)
                },
                hideLayer: function() {
                    this.isShowLayer = !1
                },
                getMsgCaptcha: function(t) {
                    var e = this
                      , a = {
                        mobile: this.mobilePhone,
                        scene: "login",
                        type: "sms"
                    };
                    t && (this.isShowLayer = !1,
                    a.captcha_code = t),
                    c.default.post(this.apihost + "/v4/mobile/verify_code/send", a).then(function(t) {
                        e.setCountdown(),
                        e.msgToken = t.validate_token,
                        e.isShowLayer = !1
                    }).catch(function(t) {
                        "NEED_CAPTCHA" === t.name ? e.showLayer() : e.$dispatch("setToast", t.message)
                    })
                },
                setCountdown: function() {
                    var t = this
                      , e = function() {
                        if (t.countdown--,
                        t.countdown <= 0)
                            return clearInterval(t.interval)
                    };
                    this.countdown = 30,
                    this.interval = setInterval(e, 1e3)
                },
                loginByMsg: function() {
                    var t = this;
                    return this.mobilePhone ? this.isMobile ? this.msgToken ? this.msgCaptcha ? void c.default.post(this.apihost + "/v1/login/app_mobile", {
                        validate_token: this.msgToken,
                        code: this.msgCaptcha,
                        mobile: this.mobilePhone
                    }).then(function() {
                        c.default.redirect()
                    }).catch(function(e) {
                        t.msgCaptcha = "",
                        t.$dispatch("setToast", e.message)
                    }) : this.$dispatch("setToast", "请填写验证码") : this.$dispatch("setToast", "请获取验证码") : this.$dispatch("setToast", "请填写合法的手机号") : this.$dispatch("setToast", "请填写手机号")
                }
            }
        }
    },
    331: function(t, e, a) {
        "use strict";
        Object.defineProperty(e, "__esModule", {
            value: !0
        });
        var s = a(140)
          , o = babelHelpers.interopRequireDefault(s)
          , i = a(1009)
          , c = babelHelpers.interopRequireDefault(i)
          , n = a(113)
          , r = babelHelpers.interopRequireDefault(n);
        e.default = {
            components: {
                Captcha: o.default,
                Switch: c.default
            },
            props: ["apihost", "current", "is-app"],
            data: function() {
                return {
                    captchaCode: "",
                    userId: "",
                    userPw: "",
                    userCaptcha: "",
                    showPwText: !1
                }
            },
            computed: {
                captchaImage: function() {
                    return this.captchaCode ? this.apihost + "/v1/captchas/" + this.captchaCode : void 0
                }
            },
            methods: {
                getCaptchaCode: function() {
                    var t = this;
                    this.userCaptcha = "",
                    r.default.post(this.apihost + "/v1/captchas").then(function(e) {
                        t.captchaCode = e.code
                    }).catch(function() {
                        return {}
                    })
                },
                loginByUserId: function() {
                    var t = this
                      , e = this.captchaCode && this.userCaptcha || !this.captchaCode;
                    return this.userId ? this.userPw ? e ? void r.default.post(this.apihost + "/v2/login", {
                        username: this.userId,
                        password: this.userPw,
                        captcha_code: this.userCaptcha
                    }).then(function() {
                        r.default.redirect()
                    }).catch(function(e) {
                        t.getCaptchaCode(),
                        t.$dispatch("setToast", e.message)
                    }) : this.$dispatch("setToast", "请填写验证码") : this.$dispatch("setToast", "密码不能为空") : this.$dispatch("setToast", "手机/邮箱/用户名 不能为空")
                }
            }
        }
    },
    332: function(t, e) {
        "use strict";
        Object.defineProperty(e, "__esModule", {
            value: !0
        }),
        e.default = {
            props: {
                value: {
                    type: Boolean,
                    twoWay: !0
                }
            },
            methods: {
                switchValue: function() {
                    this.value = !this.value
                }
            }
        }
    },
    448: function(t, e) {},
    449: function(t, e) {},
    450: function(t, e) {},
    559: function(t, e) {},
    581: function(t, e) {},
    788: function(t, e) {
        t.exports = " <div> <eleme-header :data=headerOpt> <span class=header-right slot=right @click=switchTo(headerOpt.switchTarget)>{{ headerOpt.switchName }}</span> </eleme-header> <component :is=current :current.sync=current :is-app=isApp :apihost=apihost></component> <div class=toast-wrap v-show=showToast transition=toast> <div class=toast-text v-text=toast></div> </div> </div> "
    },
    789: function(t, e) {
        t.exports = " <div> <div class=form> <div class=form-line> <input type=tel maxlength=11 placeholder=手机号 v-model=mobilePhone> <div class=msg-form-btn v-if=countdown>已发送({{ countdown }}s)</div> <div v-else class=msg-form-btn :class=\"{ 'msg-form-enable': enableShowLayer }\" @click=getMsgCaptcha()>{{ buttonText }} </div> </div> <div class=form-line> <input type=text maxlength=8 placeholder=验证码 v-model=msgCaptcha> </div> </div> <div class=msg-tips> 温馨提示：未注册饿了么帐号的手机号，登录时将自动注册，且代表您已同意<a href=//h5.ele.me/service/agreement/ target=_blank>《用户服务协议》</a> </div> <div class=form-login @click=loginByMsg>登录</div> <div class=password-forget v-if=isApp> <a href=javascript: @click=\"current = 'password'\">密码登录</a> </div> <captcha v-show=isShowLayer :apihost=apihost :confirm-button=getMsgCaptcha :cancel-button=hideLayer> </captcha> </div> "
    },
    790: function(t, e) {
        t.exports = " <div class=form> <div class=form-line> <input type=text placeholder=手机/邮箱/用户名 v-model=userId> </div> <div class=form-line> <input placeholder=密码 v-model=userPw :type=\"showPwText ? 'text' : 'password'\"> <switch :value.sync=showPwText></switch> </div> <div class=form-line v-if=captchaCode> <input type=text placeholder=验证码 v-model=userCaptcha> <img class=password-img-captcha :src=captchaImage @click=getCaptchaCode()> <div class=password-img-change @click=getCaptchaCode()> 看不清<br> <span>换一张</span> </div> </div> <div class=\"form-login password-login\" @click=loginByUserId()>登录</div> <div class=password-forget> <a href=javascript: @click=\"current = 'message'\" v-if=isApp>短信登录</a> <a href=//m.ele.me/forget/ target=_blank>忘记密码？</a> </div> </div> "
    },
    878: function(t, e) {
        t.exports = ' <div class=captcha-mask _v-49aa31ab=""> <div class=captcha-wrap _v-49aa31ab=""> <h3 _v-49aa31ab="">请填写图形验证码</h3> <div class=captcha-form _v-49aa31ab=""> <input type=text maxlength=10 v-model=userCaptcha @keyup.enter=submitCaptcha() _v-49aa31ab=""> <img :src=captchaImage @click=reloadCaptcha() _v-49aa31ab=""> </div> <div class=captcha-confirm _v-49aa31ab=""> <span @click=cancelButton() _v-49aa31ab="">取消</span> <span @click=submitCaptcha() _v-49aa31ab="">确定</span> </div> </div> </div> '
    },
    900: function(t, e) {
        t.exports = ' <div class=switch-wrap :class="{ \'switch-true\': value }" @click=switchValue() _v-6afc61da=""> <div class=switch-cycle _v-6afc61da=""></div> <div class=switch-left _v-6afc61da="">abc</div> <div class=switch-right _v-6afc61da="">···</div> </div> '
    },
    1006: function(t, e, a) {
        var s, o, i = {};
        a(450),
        s = a(328),
        o = a(788),
        t.exports = s || {},
        t.exports.__esModule && (t.exports = t.exports.default);
        var c = "function" == typeof t.exports ? t.exports.options || (t.exports.options = {}) : t.exports;
        o && (c.template = o),
        c.computed || (c.computed = {}),
        Object.keys(i).forEach(function(t) {
            var e = i[t];
            c.computed[t] = function() {
                return e
            }
        })
    },
    1007: function(t, e, a) {
        var s, o, i = {};
        a(448),
        s = a(330),
        o = a(789),
        t.exports = s || {},
        t.exports.__esModule && (t.exports = t.exports.default);
        var c = "function" == typeof t.exports ? t.exports.options || (t.exports.options = {}) : t.exports;
        o && (c.template = o),
        c.computed || (c.computed = {}),
        Object.keys(i).forEach(function(t) {
            var e = i[t];
            c.computed[t] = function() {
                return e
            }
        })
    },
    1008: function(t, e, a) {
        var s, o, i = {};
        a(449),
        s = a(331),
        o = a(790),
        t.exports = s || {},
        t.exports.__esModule && (t.exports = t.exports.default);
        var c = "function" == typeof t.exports ? t.exports.options || (t.exports.options = {}) : t.exports;
        o && (c.template = o),
        c.computed || (c.computed = {}),
        Object.keys(i).forEach(function(t) {
            var e = i[t];
            c.computed[t] = function() {
                return e
            }
        })
    },
    1009: function(t, e, a) {
        var s, o, i = {};
        a(581),
        s = a(332),
        o = a(900),
        t.exports = s || {},
        t.exports.__esModule && (t.exports = t.exports.default);
        var c = "function" == typeof t.exports ? t.exports.options || (t.exports.options = {}) : t.exports;
        o && (c.template = o),
        c.computed || (c.computed = {}),
        Object.keys(i).forEach(function(t) {
            var e = i[t];
            c.computed[t] = function() {
                return e
            }
        })
    }
});
