<template>
    <div class="restContainer">
        <head-top head-title="重置密码" goBack="true"></head-top>
        <form class="restForm">
            <section class="input_container">
                <input type="text" placeholder="手机号/邮箱" v-model="userAccount">
            </section>
            <section class="input_container">
                <input type="text" placeholder="请输入旧密码" name="oldPassWord" v-model="oldPassWord">
            </section>
            <section class="input_container">
                <input type="text" placeholder="请输入新密码" name="newPassWord" v-model="newPassWord">
            </section>
            <section class="input_container">
                <input type="text" placeholder="请确认密码" name="confirmPassWord" v-model="confirmPassWord">
            </section>
            <section class="input_container captcha_code_container">
                <input type="text" placeholder="验证码" maxlength="4" v-model="codeNumber">
                <div class="img_change_img">
                    <img v-show="captchaCodeImg" :src="captchaCodeImg">
                    <div class="change_img" @click="getCaptchaCode">
                        <p>看不清</p>
                        <p>换一张</p>
                    </div>
                </div>
            </section>
        </form>
        <div class="login_container" @click="resetButton">确认修改</div>
        <alert-tip v-if="showAlert" :showHide="showAlert" @closeTip="closeTip" :alertText="alertText"></alert-tip>
    </div>
</template>

<script>
    import headTop from '../../components/header/head'
    import alertTip from '../../components/common/alertTip'
    import {mapState, mapMutations} from 'vuex'
    import {getcaptchas, checkExsis, sendMobile} from '../../service/getData'

    export default {
        data(){
            return {
                userAccount: null, //帐号
                oldPassWord: null, //旧密码
                newPassWord: null, //新密码
                confirmPassWord: null, //确认密码
                captchaCodeImg: null, //验证码地址
                codeNumber: null, //验证码
                showAlert: false, //显示提示组件
                alertText: null, //提示的内容
                accountType: null,//注册方式
            }
        },
        created(){
            this.getCaptchaCode();
        },
        components: {
            headTop,
            alertTip,
        },
        methods: {
            ...mapMutations([
                'RECORD_USERINFO',
            ]),
            async getCaptchaCode(){
                let res = await getcaptchas();
                this.captchaCodeImg = 'https://mainsite-restapi.ele.me/v1/captchas/' + res.code;
            },
            async resetButton(){
                if (!this.userAccount) {
                    this.showAlert = true;
                    this.alertText = '请输入您的帐号';
                    return
                }else if(!this.oldPassWord){
                    this.showAlert = true;
                    this.alertText = '请输入旧密码';
                    return
                }else if(!this.newPassWord){
                    this.showAlert = true;
                    this.alertText = '请输入新密码';
                    return
                }else if(!this.confirmPassWord){
                    this.showAlert = true;
                    this.alertText = '请输确认密码';
                    return
                }else if(!this.codeNumber){
                    this.showAlert = true;
                    this.alertText = '请输验证码';
                    return
                }else if(/^1\d{10}$/gi.test(this.userAccount)){
                    this.accountType = 'mobile';
                    let res = await checkExsis(this.userAccount, this.accountType);
                    if (res.message) {
                        this.showAlert = true;
                        this.alertText = res.message;
                        return
                    }else if(!res.is_exists) {
                        this.showAlert = true;
                        this.alertText = '您输入的手机号尚未绑定';
                        return
                    }
                }else if(/@/gi.test(this.userAccount)){
                    this.accountType = 'email';
                    let res = await checkExsis(this.userAccount, this.accountType);
                    if (res.message) {
                        this.showAlert = true;
                        this.alertText = res.message;
                        return
                    }else if(!res.is_exists) {
                        this.showAlert = true;
                        this.alertText = '您输入的邮箱尚未绑定';
                        return
                    }
                }else{
                    this.showAlert = true;
                    this.alertText = '请输入正确的手机号/邮箱';
                    return
                }
                let res = await sendMobile(this.userAccount, this.codeNumber, this.accountType);
                if (res.message) {
                    this.showAlert = true;
                    this.alertText = res.message;
                    return
                }
            },
            closeTip(){
                this.showAlert = false;
            }   
        }
    }

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    .restContainer{
        padding-top: 1.95rem;
    }
    .restForm{
        background-color: #fff;
        margin-top: .6rem;
        .input_container{
            display: flex;
            justify-content: space-between;
            padding: .6rem .8rem;
            border-bottom: 1px solid #f1f1f1;
            input{
                @include sc(.7rem, #666);
                flex: 1;
            }

        }
        .captcha_code_container{
            height: 2.2rem;
            .img_change_img{
                display: flex;
                align-items: center;
                img{
                    @include wh(3.5rem, 1.5rem);
                    margin-right: .2rem;
                }
                .change_img{
                    display: flex;
                    flex-direction: 'column';
                    flex-wrap: wrap;
                    width: 2rem;
                    justify-content: center;
                    p{
                        @include sc(.55rem, #666);
                    }
                    p:nth-of-type(2){
                        color: #3b95e9;
                        margin-top: .2rem;
                    }
                }
            }
        }
    }
    .login_container{
        margin: 1rem .5rem;
        @include sc(.7rem, #fff);
        background-color: #4cd964;
        padding: .5rem 0;
        border: 1px;
        border-radius: 0.15rem;
        text-align: center;
    }
</style>
