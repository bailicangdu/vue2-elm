 <template>
    <div class="page">
        <head-top head-title="余额问题" go-back='true'></head-top>
        <div class="markdown"><h3 id="q1-">Q1: 使用余额的条件</h3>
        <p>为了保护账户安全，使用余额前必须先绑定手机号。</p>
        <h3 id="q2-">Q2: 余额可以怎么用？</h3>
        <p>余额可以在饿了么平台上提现，当余额大于等于待支付金额时可以在支持在线支付的商家中进行消费。提现功能将于2016年12月25日00:00全面开放。</p>
        <h3 id="q3-">Q3:我要如何提现？</h3>
        <p>为了保护账户和资金安全，您在提现前需要输入真实姓名和用该姓名开通的银行卡号、选择开户行，并验证已绑定饿了么账号的手机号。每日只能提现1次，每次限额50元。若提现金额超过50元，点击【提现】时系统会提示您已超额，请您修改提现金额。</p>
        <h3 id="q4-">Q4:为什么会提现失败？</h3>
        <p>可能原因有：您的姓名、开户行、银行卡号等信息不匹配；您当日的提现次数和金额超过限制；您的账户存在异常，被风控拦截。</p>
        </div>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {payRequest} from 'src/service/getData'
    import alertTip from 'src/components/common/alertTip'
    import loading from 'src/components/common/loading'

    export default {
      data(){
            return{
               
            }
        },
        components: {
            headTop,
        },
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .page{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        padding-top: 1.95rem;
        z-index: 203;
        background-color: #fff;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .markdown{
        @include sc(.6rem, #666);
        padding: 0 0.5rem 0.5rem;
        h3{
            line-height: 2rem;
        }
        p,li{
            @include sc(.6rem, #666);
            line-height: 1rem;
        }
    }
</style>
