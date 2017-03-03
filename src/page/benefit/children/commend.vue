 <template>
  <div class="page">
        <head-top head-title="推荐有奖" go-back='true'></head-top>
        <section class="activity_banner">
            <img src="../../../images/activity.png">
        </section>
        <section class="invite_firend">
            <div class="invite_firend_style">
                <img src="../../../images/weixin.png" @click="fenxiang">
                <p>邀请微信好友</p>
            </div>
            <div class="invite_firend_style">
                <img src="../../../images/qq.png" @click="fenxiang">
                <p>邀请QQ好友</p>
            </div>
            <div class="invite_firend_style">
                <img src="../../../images/fenxiang.png" @click="fenxiang">
                <p>面对面邀请</p>
            </div>
        </section>
        <section class="invite_num">
            <div class="invite_num_style">
                <p>累计收益</p>
                <p><span>0</span>元</p>
            </div>
            <div class="invite_num_style invite_people">
                <p>成功邀请</p>
                <p><span>0</span>人</p>
            </div>
        </section>
        <p class="income_detail">-收益明细-</p>
        <section class="incom_tips">
            <img src="../../../images/qianbao.png">
            <p>还不赶紧去邀请好友</p>
        </section>
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState} from 'vuex'
    import {payRequest} from 'src/service/getData'
    import alertTip from 'src/components/common/alertTip'

    export default {
      data(){
            return{
                showAlert: false,
                alertText: null,
            }
        },
        mounted(){
            
        },
        components: {
            headTop,
            alertTip,
        },
        computed: {
            ...mapState([
                'userInfo',
            ]),
        },
        methods: {
            fenxiang(){
                this.showAlert = true;
                this.alertText = '请在饿了么APP中打开';
            }
        }
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
        overflow-y: auto;
        padding-top: 1.95rem;
        z-index: 203;
        background-color: #f5f5f5;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .activity_banner{
        img{
            @include wh(100%, 10rem);
        }
    }
    .invite_firend{
        display: flex;
        padding: 1rem 0;
        background-color: #fff;
        .invite_firend_style{
            flex: 1;
            text-align: center;
            img{
                @include wh(2.5rem, 2.5rem);
            }
            p{
                @include sc(.5rem, #333);
            }
        }
    }
    .invite_num{
        display: flex;
        margin-top: 1rem;
        @include sc(.5rem, #666);
        .invite_num_style{
            flex: 1;
            text-align: center;
            p{
                color: #666;
            }
            span{
                @include sc(.8rem, #ff5633);
                font-weight: bold;
            }
        }
        .invite_people{
            border-left: 0.025rem solid #ddd;
            span{
                @include sc(.8rem, #666);
                font-weight: bold;
            }
        }
    }
    .income_detail{
        text-align: center;
        margin-top: 1rem;
        @include sc(.5rem, #666);
    }
    .incom_tips{
        text-align: center;
        margin-top: 1rem;
        img{
            @include wh(1.3rem, 1.6rem);
        }
        p{
            @include sc(.5rem, #999);
        }
    }
</style>
