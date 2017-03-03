 <template>
    <div class="page">
        <head-top head-title="积分问题" go-back='true'></head-top>
        <div class="markdown">
            <h3 id="q1-">Q1: 怎么获得积分？</h3>
            <p>在线支付的订单将获得订单积分奖励：</p>
            <ul>
                <li>积分将在用户完成评价后获得。</li>
                <li>可获得积分=订单金额×10（即1元=10点积分）。</li>
                <li>订单金额指实际付款金额，不包含活动优惠金额。</li>
                <li>每位用户每天最多可以获得2000积分，体验商家的订单和评价不会增加积分。</li>
            </ul>
            <h3 id="q2-">Q2: 积分用来做什么？</h3>
            <p>可以在积分商城兑换各种礼品。</p>
            <h3 id="q3-">Q3: 礼品兑换很多天了还没有收到，该怎么办？</h3>
            <p>礼品从兑换日起，3个工作日（周末不算）内处理发货，发货后，通常会在3个工作日左右送达。</p>
            <h3 id="q4-">Q4: 礼品兑换中的手机充值卡兑换，怎么样进行充值，充值之前会和我电话确认嘛？</h3>
            <p>不会，手机充值卡兑换，是直接充值到您填写的手机号上，充值之前不会和您电话确认，所以您在填写电话的时候，请确认电话是否正确。</p>
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
