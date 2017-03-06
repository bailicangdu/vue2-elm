 <template>
    <div class="validation_page">
        <head-top head-title="用户手机验证" go-back='true'></head-top>
        <section class="validataion_container">
            <div class="voice_tip" v-if="showVoiceTip">
                <p>电话拨打中...</p>
                <p>请留意来自 <span>10105757</span> 或者 <span>021-315754XX</span> 的电话</p>
            </div>
            <header class="validataion_header">
                <span>收不到短信？使用</span>
                <span @click="sendVoice">语音验证码</span>
            </header>
            <form class="input_form">
                <input type="text" name="validate" v-model="validate" placeholder="验证码" maxlength="6">
                <span class="disable" v-if="countDown">{{countDown}} S</span>
                <span class="repost" v-else @click="recall">重新发送</span>
            </form>
        </section>
        <div class="determine" @click="confrimOrder">确定</div>
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {rePostVerify, validateOrders} from 'src/service/getData'
    import alertTip from 'src/components/common/alertTip'

    export default {
      data(){
            return{
               	validate: null, //验证码
                countDown: 60, //倒计时
                sig: null, //sig值
                reCallVerify: null, //重发验证信息
                showAlert: false, 
                alertText: null,
                showVoiceTip: false, //显示语音验证
                type: 'sms',
            }
        },
        components: {
            headTop,
            alertTip,
        },
        created(){
            this.sig = this.$route.query.sig;
        },
        mounted(){
            this.count();
            this.getData();
        },
        beforeDestroy(){
            clearInterval(this.timer);
        },
        props:[],
        computed: {
            ...mapState([
                'needValidation', 'cart_id', 'sig', 'orderParam'
            ]),
        },
        methods: {
            ...mapMutations([
                'CHANGE_ORDER_PARAM', 'ORDER_SUCCESS'
            ]),
            //到计时
            count(){
                this.countDown = 60;
                clearInterval(this.timer);
                this.timer = setInterval(() => {
                    this.countDown -- ;
                    if (this.countDown == 0) {
                        clearInterval(this.timer);
                    }
                }, 1000);
            },
            //重新发送
            recall(){
                this.count();
                this.type = 'sms';
                this.getData();
            },
            //发送语音验证
            sendVoice(){
                this.showVoiceTip = true;
                this.type = 'voice';
                this.getData();
            },
            //获取验证信息
            async getData(){
                this.reCallVerify = await rePostVerify(this.cart_id, this.sig, this.type);
                if (this.reCallVerify.message) {
                    this.showAlert = true;
                    this.alertText = this.reCallVerify.message;
                }
            },
            //确认订单
            async confrimOrder(){
                this.CHANGE_ORDER_PARAM({validation_code: this.validate, validation_token: this.reCallVerify.validate_token})
                let orderRes = await validateOrders(this.orderParam);
                //如果信息错误则提示，否则进入付款页面
                if (orderRes.message) {
                    this.showAlert = true;
                    this.alertText = orderRes.message;
                    return
                }
                this.ORDER_SUCCESS(orderRes);
                this.$router.push('/confirmOrder/payment');
            },
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .validation_page{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #f5f5f5;
        z-index: 204;
        padding-top: 1.95rem;
        p, span, input{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .determine{
        background-color: #4cd964;
        @include sc(.7rem, #fff);
        text-align: center;
        margin: 0 .7rem;
        line-height: 1.8rem;
        border-radius: 0.2rem;
        margin-top: 0.5rem;
    }
    .validataion_container{
        background-color: #fff;
        padding: .7rem;
        .validataion_header{
            span{
                @include sc(.7rem, #333);
            }
            span:nth-of-type(2){
                color: #ff6000;
            }
        }
    }
    .input_form{
        display: flex;
        padding: .7rem 0;
        *{
            @include sc(.65rem, #666);
            border-radius: 0.15rem;
        }
        input{
            flex: 3;
            height: 1.5rem;
            background-color: #eee;
            margin-right: .8rem;
            padding: 0 .6rem;
        }
        span{
            flex: 1;
            height: 1.5rem;
            display: inline-block;
            text-align: center;
            line-height: 1.5rem;
            font-size: .6rem;
        }
        .repost{
            background-color: $blue;
            color: #fff;
        }
        .disable{
            background-color: #eee;
            color: #999;
        }
    }
    .voice_tip{
        margin-bottom: .4rem;
        p{
            @include sc(.65rem, #333);
            line-height: 1rem;
            span{
                color: #ff6000;
            }
        }
    }
</style>
