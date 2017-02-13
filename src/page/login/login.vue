<template>
    <div class="loginContainer">
        <head-top :head-title="loginWay? '登录':'密码登录'" goBack="true">
            <div slot="changeLogin" class="change_login" @click="changeLoginWay">{{loginWay? "密码登陆":"短信登陆"}}</div>
        </head-top>
        <form class="loginForm">
            <div class="input_container phone_number">
                <input type="text" placeholder="手机号">
                <button>获取验证码</button>
            </div>
            <div class="input_container">
                <input type="text" placeholder="验证码">
            </div>
        </form>
        <p class="login_tips">
            温馨提示：未注册饿了么账号的手机号，登陆时将自动注册，且代表您已同意
            <a href="https://h5.ele.me/service/agreement/">《用户服务协议》</a>
        </p>
        <div class="login_container">
            <button class="login_button">登陆</button>  
        </div>
    </div>
</template>

<script>
    import headTop from '../../components/header/head'

    export default {
        data(){
            return {
                loginWay: true,
            }
        },
        components: {
            headTop,
        },
        methods: {
            changeLoginWay(){
                this.loginWay = !this.loginWay;
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
                padding: .3rem .4rem;
                border: 1px;
                border-radius: 0.15rem;
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
        display: flex;
        margin: 0 .5rem;
        .login_button{
            flex: 1;
            @include sc(.7rem, #fff);
            background-color: #4cd964;
            padding: .5rem 0;
            border: 1px;
            border-radius: 0.15rem;
        }
    }
</style>
