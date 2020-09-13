/*
http://aaa.uestc.edu.cn/static/js/main.js?v=2.00.20181212
*/
$(function () {
    var host = location.protocol + "//";
    if (isIPV6) {
        if (portal.AuthIP6 != "") {
            portal.AuthIP6 = "[" + portal.AuthIP6 + "]";
        }
        host += portal.AuthIP6 || location.hostname;
    } else {
        host += portal.AuthIP || location.hostname;
    }

    //show error
    function showErrorMessage(error, success, redirect) {
        var icon = 2;
        if (typeof success != "undefined" || success) {
            icon = 1
        }
        var message = error;
        if (typeof(translate[error]) != "undefined") {
            message = translate[error];
        }
        if (typeof redirect != "undefined" && redirect) {
            layer.alert(message, {
                icon: icon,
                skin: 'layui-layer-molv',
                btn: [(translate['OK'] || "OK")],
                title: translate['Info'] || "Info"
            }, function () {
                //location.href = "./";
                window.history.go(-1);
            });
            return;
        }
        layer.alert(message, {
            icon: icon,
            skin: 'layui-layer-molv',
            btn: [(translate['OK'] || "OK")],
            title: translate['Info'] || "Info"
        });
    }

    function getQueryString(name) {
        var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i");
        var r = window.location.search.substr(1).match(reg);
        if (r != null) return unescape(r[2]);
        return "";
    }

    //show log
    function showLog(username) {
        $.Log({username: username}, function (data) {
            var message = data.Message;
            var error = "NoResponseDataError";
            if (data.Message != "") {
                if (data.Message.indexOf("E") == 0) {
                    error = data.Message.substr(0, 5);
                } else {
                    error = data.Message;
                }
            }
            if (error != "E2901" && typeof(translate[error]) != "undefined") {
                message = translate[error];
            }
            layer.alert(message, {
                icon: 2,
                skin: 'layui-layer-molv',
                btn: [(translate['OK'] || "OK")],
                title: translate['Info'] || "Info"
            });
        });
    }

    //if login to success page
    if (typeof success == "undefined" || !success) {
        if ((typeof wechat == "undefined" || !wechat) && (typeof msg == "undefined" || !msg)) {
            var wait = getQueryString("srun_wait");
            if (wait == "") {
                $.Info(host, {}, function (data) {
                    if (data.error == "ok") {
                        location.href = "./srun_portal_success" + location.search;
                    }
                });
            }
        }
    }

    //self-service
    $("#self-service").click(function () {
        var username = $("#username").val(),
            password = $("#password").val();
        if (typeof username != "undefined" && username != "") {
            var ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
                base64 = $.base64;
            base64.setAlpha(ALPHA);
            var data = "";
            if (typeof password != "undefined" && password != "") {
                var pwd = md5(password);
                data = base64.encode(username + ":" + pwd);
            }
            if (typeof success != "undefined" && success) {
                data = base64.encode(username + ":" + username);
            }
            if (data != "") {
                window.open(location.protocol + "//" + (portal.ServiceIP || location.hostname) + ":8800/site/sso?data=" + data);
                return;
            }
            window.open(location.protocol + "//" + (portal.ServiceIP || location.hostname) + ":8800");
            return;
        }
        window.open(location.protocol + "//" + (portal.ServiceIP || location.hostname) + ":8800");
    });

    //Change Language
    $("#language").click(function () {
        var language = $(this).text();
        var l = "zh-CN";
        if (typeof language != "undefined" && language == "English") {
            l = "en-US";
        } else {
            if (typeof lang != "undefined") {
                l = lang == "zh-CN" ? "en-US" : "zh-CN";
            }
        }
        $.Language(l);
    });

    var messageUri = "/v2/srun_portal_message";
    if (portal.MsgApi == "old") {
        messageUri = "/v1/srun_portal_message";
    }
    //Message
    $.Message(host, messageUri, {}, function (data) {
        if (data.Code == 0 && data.Data != null) {
            if (data.Data.length > 0) {
                if (portal.MsgApi == "old") {
                    $("#notice-title").text(data.Data[0].msg_head);
                    $("#notice-content").html(data.Data[0].msg_con);
                    return;
                }
                $("#notice-title").text(data.Data[0].Title);
                $("#notice-content").html(data.Data[0].Content);
            }
        }
    });

    function IpAlreadyRetryAuth(params) {
        var logoutParms = {
            ac_id: params.acid,
            ip: params.ip
        }
        $.Logout(host, logoutParms, function (data) {
            if (data.error == "ok") {
                $.Login(host, params, function (resp) {
                    if (resp.error == "ok") {
                        location.href = "./srun_portal_success" + location.search;
                        return;
                    }
                    showErrorMessage(resp.message);
                });
            } else {
                //Show Error Message
                showErrorMessage(data.message);
            }
        });
    }

    //Login
    $("#login").click(function () {
        //username is empty
        var username = $("#username").val();
        if (username == "") {
            $("#username").focus();
            return;
        }
        //password is empty
        var password = $("#password").val();
        if (password == "") {
            $("#password").focus();
            return;
        }
        var acid = $("#ac_id").val(),
            ip = $("#user_ip").val();
        var params = {
            //username:$.trim(username).toLowerCase(),
            username: $.trim(username),
            domain: "",
            password: password,
            ac_id: acid,
            ip: ip,
            double_stack: 0
        };
        if ($("#domain").val() != undefined) {
            params.domain = $("#domain").val();
        }
        var ua = window.navigator.userAgent;
        var md = new MobileDetect(ua);
        var mobile = md.mobile();
        if ((!mobile && portal.DoubleStackPC) || (mobile && portal.DoubleStackMobile)) {
            params.double_stack = 1;
        }
        $.Login(host, params, function (data) {
            if (data.error == "ok") {
                /*if( (!mobile && portal.DoubleStackPC) || (mobile && portal.DoubleStackMobile)) {
                    var doubleHost = location.protocol + "//";
                    if (isIPV6) {
                        doubleHost += portal.AuthIP||location.hostname;
                    } else {
                        if (portal.AuthIP6 != "") {
                            portal.AuthIP6 = "["+portal.AuthIP6+"]";
                        }
                        doubleHost += portal.AuthIP6||location.hostname;
                    }
                    params.double_stack = 0;
                    params.ip = "";
                    $.Login(doubleHost, params, function(data) {
                        //Redirect Success Page
                        location.href = "./srun_portal_success"+location.search + "&srun_domain=" + params.domain;
                    });
                } else {*/
                location.href = "./srun_portal_success" + location.search + "&srun_domain=" + params.domain;
                //}
            } else {
                //先下线在上线
                if (data.message == "IpAlreadyOnlineError") {
                    IpAlreadyRetryAuth(params);
                } else {
                    if (data.message == "NoResponseDataError") {
                        showLog(username);
                        return;
                    }
                    //Show Error Message
                    if (data.message == "NotOnlineError") {
                        setTimeout(function () {
                            location.href = "./srun_portal_success" + location.search;
                        }, 1500);
                    } else {
                        showErrorMessage(data.message, false, true);
                    }
                }
            }
        });
    });
    //Logout DM
    $("#logout-dm").click(function () {
        //username is empty
        var username = $("#username").val();
        if (username == "") {
            $("#username").focus();
            return;
        }
        var params = {
            username: username,
            domain: "",
            ac_id: $("#ac_id").val(),
            ip: $("#user_ip").val()
        };
        if ($("#domain").val() != undefined) {
            params.domain = $("#domain").val();
        }
        $.DM(host, params, function (data) {
            if (data.error == "ok") {
                //Show DM Logout OK!
                showErrorMessage("LogoutOK", true);
            } else {
                if (data.message == "NoResponseDataError") {
                    showLog(username);
                    return;
                }
                //Show Error Message
                showErrorMessage(data.message);
            }
        });
    });
    //Logout
    $("#logout").click(function () {
        var acid = $("#ac_id").val(),
            username = $("#username").val();
        var params = {
            username: username,
            ac_id: acid,
            ip: $("#user_ip").val(),
            domain: $("#domain").val()
        };
        $.Logout(host, params, function (data) {
            if (data.error == "ok") {
                isLogin = false;
                //Redirect Login Page
                //location.href="./index_" + acid + ".html?srun_wait=1"; 注释：李文宇
                location.href = "./index_" + acid + ".html";
            } else {
                if (data.message == "NoResponseDataError") {
                    showLog(username);
                    return;
                }
                //Show Error Message
                showErrorMessage(data.message);
            }
        });
    });
    //Get Verify Code
    //clearTimeout(loading);//停止倒计时
    $("#code").click(function () {
        $this = $(this);
        if ($this.hasClass("disabled")) {
            return false;
        }
        //phone is phone?
        var phone = $("#username").val();
        if (phone == "") {
            $("#username").focus();
            return;
        }
        var ip = $("#user_ip").val();
        var mac = $("#user_mac").val();
        var wait = 60, loading;

        function time(t) {
            if (t == 1) {
                $this.text(translate["GetVerifyCode"] || "获取验证码");
                $this.removeClass("disabled");
            } else {
                t--;
                $this.text(t + (translate["S"] || "秒"));
                loading = setTimeout(function () {
                    time(t)
                }, 1000);
            }
        }

        $this.addClass("disabled");
        time(wait);
        var params = {
            phone: phone,
            ip: ip,
            mac: mac
        };
        $.GetVerifyCode(host, params, function (data) {
            if (data.error == "ok") {
                //Show Success Message
                showErrorMessage("SendVerifyCodeOK", true);
            } else {
                //Stop
                clearTimeout(loading);
                $this.text(translate["GetVerifyCode"] || "获取验证码");
                $this.removeClass("disabled");
                //Show Error Message
                showErrorMessage(data.message);
            }
        });
    });
    //SMS Auth
    $("#sms-login").click(function () {
        //phone is phone?
        var phone = $("#username").val();
        if (phone == "") {
            $("#username").focus();
            return;
        }
        //vcode is empty
        var vcode = $("#vcode").val();
        if (vcode == "") {
            $("#vcode").focus();
            return;
        }
        var ip = $("#user_ip").val();
        var mac = $("#user_mac").val();
        var ac_id = $("#ac_id").val();
        var params = {
            phone: phone,
            ip: ip,
            mac: mac,
            vcode: vcode,
            ac_id: ac_id
        };
        $.SmsAuth(host, params, function (data) {
            if (data.error == "ok") {
                //Redirect Success Page
                location.href = "./srun_portal_success" + location.search;
            } else {
                if (data.message == "NoResponseDataError") {
                    showLog("smpv_" + phone);
                    return;
                }
                //Show Error Message
                showErrorMessage(data.message);
            }
        });
    });
    //Success Page
    if (typeof success != "undefined" && success) {
        $.Info(host, {}, function (data) {
            if (data.error == "ok") {
                $.Detect(host, function (response) {
                    if (response.Redirect) {
                        if ($.isMobile() && response.Mobile != "") {
                            location.href = response.Mobile;
                        } else if (response.Pc != "") {
                            location.href = response.Pc;
                        }
                    }
                });
                $("#username").val(data.user_name);
                $("#user_name").html(data.user_name);
                $("#used_flow").html(data.used_flow);
                $("#used_time").html(data.used_time);
                $("#balance").html(data.balance);
                $("#ip").html(data.ip);
                var domain = getQueryString("srun_domain");
                if (domain != "" && data.domain != "" && data.user_name.indexOf(data.domain) == -1) {
                    $("#domain").val("@" + data.domain);
                }
            } else {
                //show error message
                showErrorMessage(data.message, false, true);
            }
        });
        if ($("#visitor-qrcode").length > 0) {
            $("#visitor-qrcode").click(function () {
                var params = {
                    uri: "/api/v1/user/token-visitors",
                    user_name: $("#username").val()
                };
                $.PortalProxy(host, params, function (response) {
                    if (response.error == "ok") {
                        layer.open({
                            title: '',
                            btn: [],
                            content: $('#formbox').html(),
                            shade: 0.7,
                            shadeClose: true
                        });
                        $("#layer-qrcode").qrcode({
                            text: JSON.stringify(response.data),
                            height: 150,
                            width: 150,
                            background: "#ffffff",
                            foreground: "#4086CE"
                        });
                    } else {
                        layer.alert(response.message, {icon: 2, skin: 'layui-layer-molv', btn: "确定", title: "信息"});
                    }
                });
            });
        }
    }
    //Qrcode Page
    if (typeof qrcode != "undefined" && qrcode) {
        var username = $("#username").val();
        var params = {
            key: $("#key").val(),
            username: username,
            ac_id: $("#ac_id").val()
        };
        $.Qrcode(host, params, function (data) {
            if (data.error == "ok") {
                //Redirect Success Page
                location.href = "./srun_portal_success?ac_id=" + params.ac_id + "&theme=" + theme;
            } else {
                //Show Error Message
                if (data.message == "NoResponseDataError") {
                    showLog(username);
                    return;
                }
                showErrorMessage(data.message);
            }
        });
    }
    //Wechat
    if (typeof wechat != "undefined" && wechat) {
        var ua = navigator.userAgent;
        var isIOS = false;
        if (ua.indexOf("iPhone") != -1 || ua.indexOf("iPod") != -1 || ua.indexOf("iPad") != -1) {   //iPhone|iPod|iPad
            isIOS = true;
        }
        var wifiConfig = {
            mac: getQueryString("rmac"),
            bssid: getQueryString("bssid"),
            token: getQueryString("sruntoken")
        };
        if (isIOS && location.search.indexOf("sruntoken") == -1) { //now request provisional release
            var params = {ac_id: acid, ip: ip};
            $.Release(host, params, function (data) {
                if (data.error == "ok") {
                    $("#call").text(translate["Wait"] || "请等待...");
                    $("#call").addClass("disabled");
                    wifiConfig = data.wifiConfig;
                    location.href = "./srun_portal_weixin" + location.search + "&rmac=" + wifiConfig.mac + "&bssid=" + wifiConfig.bssid + "&sruntoken=" + wifiConfig.token;
                    return;
                }
                //show error message
                showErrorMessage(data.message);
                //disabled
                $("#call").addClass("disabled");
            });
        }
        $("#call").click(function () {
            if ($(this).hasClass("disabled")) {
                showErrorMessage("IsEvokingWeChat");
                return false;
            }
            $("#call").addClass("disabled");
            if (!isIOS) {//no ios,request provisional release and auth
                var params = {ac_id: acid, ip: ip, ssid: ssid};
                $.WeixinRelease(host, params, function (data) {
                    if (data.error == "ok") {
                        config = data.wifiConfig;
                        Wechat_GotoRedirect(
                            config.appid,
                            config.extend,
                            config.timestamp,
                            config.sign,
                            config.shop_id,
                            config.authUrl,
                            config.mac,
                            config.ssid,
                            config.bssid
                        );
                        return;
                    }
                    //show error message
                    showErrorMessage(data.message);
                });
            } else {
                //weixin auth
                var params = {
                    ac_id: acid,
                    ip: ip,
                    ssid: ssid,
                    bssid: wifiConfig.bssid || "",
                    mac: wifiConfig.mac || "",
                    token: wifiConfig.token || ""
                };
                $.WeiXinCall(host, params, function (data) {
                    if (data.error == "ok") {
                        config = data.wifiConfig;
                        Wechat_GotoRedirect(
                            config.appid,
                            config.extend,
                            config.timestamp,
                            config.sign,
                            config.shop_id,
                            config.authUrl,
                            config.mac,
                            config.ssid,
                            config.bssid
                        );
                        return;
                    }
                    //show error message
                    showErrorMessage(data.message);
                });
            }
        });
    }
    //Cas
    if (typeof cas != "undefined" && cas) {
        $.CAS(function (data) {
            if (data.error == "ok") {
                //Redirect success page
                location.href = "./srun_portal_success?ac_id=" + data.ac_id;
            } else {
                if (data.code == 301) {
                    location.href = data.redirect;
                }
                //Show Error Message
                showErrorMessage(data.message);
            }
        });
    }

    function utf16to8(str) {
        var out, i, len, c;
        out = "";
        len = str.length;
        for (i = 0; i < len; i++) {
            c = str.charCodeAt(i);
            if ((c >= 0x0001) && (c <= 0x007F)) {
                out += str.charAt(i);
            } else if (c > 0x07FF) {
                out += String.fromCharCode(0xE0 | ((c >> 12) & 0x0F));
                out += String.fromCharCode(0x80 | ((c >> 6) & 0x3F));
                out += String.fromCharCode(0x80 | ((c >> 0) & 0x3F));
            } else {
                out += String.fromCharCode(0xC0 | ((c >> 6) & 0x1F));
                out += String.fromCharCode(0x80 | ((c >> 0) & 0x3F));
            }
        }
        return out;
    }

    //Qrcode
    var socket = false;
    $(".login-table a").on('click', function () {
        if ($(this).hasClass("checked") !== true) {
            $('.login-table a').removeClass("checked");
            $(this).addClass("checked");
        }
        if ($(this).parent().hasClass("login-table-l") !== true) {
            $("#out-qrcode").html("");
            $('#login-form').addClass("hidden");
            $('#out-qrcode').removeClass("hidden");
            var info = {
                "ac_id": $("#ac_id").val(),
                "ip": $("#user_ip").val()
            };
            var ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
                base64 = $.base64;
            base64.setAlpha(ALPHA);
            i = base64.encode(JSON.stringify(info));
            var params = {
                "Tips": utf16to8("请在'迅连'小程序中扫码认证"),
                "info": i
            };
            var text = JSON.stringify(params);
            var $li_1 = $("<div id='out-qrcode1'class='col-xs-12 col-md-6' style='float: left;'><div class='out-qrcode1'></div><p class='text-center'style=\"font-size:16px;\">第二步:小程序中认证后<br/>点击右上角扫这里</p></div>");
            var $parent = $("#out-qrcode");
            $parent.append($li_1);
            $('.out-qrcode1').qrcode({
                text: text,
                height: 150,
                width: 150,
                src: '/static/images/basic/qrcode-logo.png',
                background: "#ffffff", //背景颜色
                foreground: "#4086CE" //前景颜色
            });
            $('#out-qrcode').prepend('<div class="col-xs-12 col-md-6" style=" float: left; text-align: center;"><img src="/static/images/basic/small-app-logo.jpg" style="width:150px;height:150px;vertical-align:inherit;"/><p class=\'text-center\' style="font-size:16px;">第一步:扫这里<br/>进入迅连小程序上网</p></div>');

            // 连接socket
            var uid = $("#user_ip").val() + $("#ac_id").val();
            socket = io('http://' + document.domain + ':2120');
            socket.on('connect', function () {
                socket.emit('login', uid);
            });
            socket.on('new_msg', function (msg) {
                console.log(msg);
            });
            socket.on('update_online_count', function (online_stat) {
                console.log(online_stat);
            });
            // 扫码登录成功时
            socket.on('qrcode_auth_success', function (data) {
                console.log(data);
                setTimeout(function () {
                    location.reload();
                }, 1000);
            });
        } else {
            // 关闭socket
            if (socket !== false) {
                socket.close();
            }
            location.reload();
        }
    });
});