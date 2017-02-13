<template>
    <div class="loginContainer">
        <head-top :head-title="loginWay? '登录':'密码登录'" goBack="true">
            <div slot="changeLogin" class="change_login" @click="changeLoginWay">{{loginWay? "密码登陆":"短信登陆"}}</div>
        </head-top>
        <form class="loginForm" v-if="loginWay">
            <div class="input_container phone_number">
                <input type="text" placeholder="手机号" name="phone" maxlength="11" v-model="phoneNumber" @input="inputPhone">
                <button @click.prevent="getVerifyCode" :class="{right_phone_number:rightPhoneNumber}" v-show="!computedTime">获取验证码</button>
                <button  @click.prevent v-show="computedTime">已发送({{computedTime}}s)</button>
            </div>
            <div class="input_container">
                <input type="text" placeholder="验证码" name="mobileCode" maxlength="6" v-model="mobileCode">
            </div>
        </form>
        <form class="loginForm" v-else>
            <div class="input_container">
                <input type="text" placeholder="手机号/邮箱/用户名">
            </div>
            <div class="input_container">
                <input :type="showPassword? 'text':'password'" placeholder="密码">
                <div class="button_switch" :class="{change_to_text: showPassword}">
                    <div class="circel_button" :class="{trans_to_right: showPassword}" @click="changePassWordType"></div>
                    <span>abc</span>
                    <span>...</span>
                </div>
            </div>
        </form>
        <p class="login_tips">
            温馨提示：未注册饿了么账号的手机号，登陆时将自动注册，且代表您已同意
            <a href="https://h5.ele.me/service/agreement/">《用户服务协议》</a>
        </p>
        <div class="login_container" @click="mobileLogin">登陆</div>
        <router-link to="/forget" class="to_forget" v-if="!loginWay">忘记密码？</router-link>
    </div>
</template>

<script>
    import headTop from '../../components/header/head'
    import {mobileCode, sendLogin} from '../../service/getData'

    export default {
        data(){
            return {
                loginWay: true,
                showPassword: false,
                phoneNumber: null,
                mobileCode: null,
                rightPhoneNumber: false,
                validate_token: null,
                computedTime: 0,
                uerInfo: null,
            }
        },
        components: {
            headTop,
        },
        methods: {
            changeLoginWay(){
                this.loginWay = !this.loginWay;
            },
            changePassWordType(){
                this.showPassword = !this.showPassword;
            },
            inputPhone(){
                if(/^1\d{10}$/gi.test(this.phoneNumber)){
                    this.rightPhoneNumber = true;
                }else{
                    this.rightPhoneNumber = false;
                }
            },
            async getVerifyCode(){
                if (this.rightPhoneNumber) {
                    this.computedTime = 30;
                    this.timer = setInterval(() => {
                        this.computedTime --;
                        if (this.computedTime == 0) {
                            clearInterval(this.timer)
                        }
                    }, 1000)
                    this.validate_token = await mobileCode(this.phoneNumber);
                }
            },
            async mobileLogin(){
                if (!this.rightPhoneNumber) {
                    alert(1)
                    return
                }else if(!(/^\d{6}$/gi.test(this.mobileCode))){
                    alert(2)
                    return
                }
                this.uerInfo = await sendLogin(this.mobileCode, this.phoneNumber, this.validate_token);
            }
        }
    }

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    .loginContainer{
        padding-top: 1.95rem;
    }
    .change_login{
        position: absolute;
        @include ct;
        right: 0.75rem;
        @include sc(.7rem, #fff);
    }

    .loginForm{
        background-color: #fff;
        margin-top: .6rem;
        .input_container{
            display: flex;
            justify-content: space-between;
            padding: .6rem .8rem;
            border-bottom: 1px solid #f1f1f1;
            input{
                @include sc(.7rem, #666);
            }
            button{
                @include sc(.65rem, #fff);
                font-family: Helvetica Neue,Tahoma,Arial;
                padding: .28rem .4rem;
                border: 1px;
                border-radius: 0.15rem;
            }
            .right_phone_number{
                background-color: #4cd964;
            }
        }
        .phone_number{
            padding: .3rem .8rem;
        }
    }
    .login_tips{
        @include sc(.5rem, #999);
        padding: .4rem .6rem;
        line-height: .7rem;
        a{
            color: #3b95e9;
        }
    }
    .login_container{
        margin: 0 .5rem 1rem;
        @include sc(.7rem, #fff);
        background-color: #4cd964;
        padding: .5rem 0;
        border: 1px;
        border-radius: 0.15rem;
        text-align: center;

    }
    .button_switch{
        background-color: #ccc;
        display: flex;
        align-items: center;
        justify-content: space-between;
        @include wh(2rem, .7rem);
        padding: 0 .2rem;
        border: 1px;
        border-radius: 0.5rem;
        position: relative;
        transition: all .3s;
        .circel_button{
            transition: all .3s;
            position: absolute;
            z-index: 1;
            @include wh(1.2rem, 1.2rem);
            left: -.1rem;
            box-shadow: 0 0.026667rem 0.053333rem 0 rgba(0,0,0,.1);
            background-color: #f1f1f1;
            border-radius: 50%;

        }
        .trans_to_right{
            transform: translateX(1.3rem);
        }
        span{
            @include sc(.45rem, #fff);
            transform: translateY(.05rem);
            line-height: .6rem;
        }
        span:nth-of-type(2){
            transform: translateY(-.08rem);
        }
    }
    .change_to_text{
        background-color: #4cd964;
    }
    .to_forget{
        float: right;
        @include sc(.6rem, #3b95e9);
        margin-right: .3rem;
    }
</style>
