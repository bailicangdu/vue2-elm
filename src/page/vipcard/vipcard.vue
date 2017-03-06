 <template>
  <div class="rating_page">
        <head-top head-title="会员中心" go-back='true'></head-top>
        <section v-if="userInfo">
            <p class="buy_for">为账户 <span>{{userInfo.username}}</span> 购买会员</p>
            <section class="vip_prerogative">
                <router-link to="/vipcard/vipDescription" class="header_style">
                    <span class="header_left">会员特权</span>
                    <section class="header_right">
                        <span>会员说明</span>  
                        <svg fill="#ccc">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg> 
                    </section>
                </router-link>
                <section class="vip_detail">
                    <div class="vip_detail_left">
                        <img src="../../images/sheng.jpeg" height="80" width="70">
                    </div>
                    <div class="vip_detail_right">
                        <h4>减免配送费</h4>
                        <p>每月减免30单，每日可减免3单，每单最高减4元</p>
                        <p>蜂鸟专送专享</p>
                    </div>
                </section>
                <section class="vip_detail">
                    <div class="vip_detail_left">
                        <img src="../../images/jifen.jpeg" height="80" width="70">
                    </div>
                    <div class="vip_detail_right">
                        <h4>减免配送费</h4>
                        <p>每月减免30单，每日可减免3单，每单最高减4元</p>
                        <p>蜂鸟专送专享</p>
                    </div>
                </section>   
            </section>
            <section class="apply_vip">
                <header class="header_style">
                    <span class="header_left">开通会员</span>
                </header>
                <section class="apply_vip_buy">
                    <div class="apply_vip_buy_left">
                        <span>1个月</span>
                        <span> ¥20</span>
                    </div>
                    <div class="apply_vip_buy_right" @click="buyCart">购买</div>
                </section>
            </section>
            <router-link to="/vipcard/useCart" class="header_style common_style">
                <span class="header_left">兑换会员</span>
                <section class="header_right">
                    <span>使用卡号卡密</span>  
                    <svg fill="#ccc">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                    </svg> 
                </section>
            </router-link>
            <router-link to="/vipcard/invoiceRecord" class="header_style common_style">
                <span class="header_left">购买记录</span>
                <section class="header_right">
                    <span>开发票</span>  
                    <svg fill="#ccc">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                    </svg> 
                </section>
            </router-link>
        </section>
        <transition name="router-slid" mode="out-in">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {getOrderList} from 'src/service/getData'

    export default {
      data(){
            return{
    
            }
        },
        mounted(){
            
        },
        computed: {
            ...mapState([
                'userInfo', 
            ]),
        },
        components: {
            headTop,
        },
        methods: {
            ...mapMutations([
                'ORDER_SUCCESS', 'BUY_CART'
            ]),
            buyCart(){
                this.ORDER_SUCCESS({order_id: '399525134200981325'});
                this.BUY_CART(20);
                this.$router.push('/confirmOrder/payment');
            },
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .rating_page{
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .buy_for{
        @include sc(.6rem, #666);
        line-height: 2rem;
        padding-left: 0.7rem;
        span{
            font-weight: bold;
        }
    }
    .vip_prerogative{
        background-color: #fff;
        padding-left: .7rem;
        margin-bottom: 0.5rem;
        .vip_detail{
            display: flex;
            border-bottom: 1px solid #f5f5f5;
            padding: .8rem 0;
            .vip_detail_left{
                margin-right: .6rem;
                img{
                    @include wh(1.6rem, 1.8rem);
                }
            }
            .vip_detail_right{
                h4{
                    @include sc(.7rem, #333);
                    font-weight: normal;
                }
                p{
                    @include sc(.5rem, #999);
                }
            }
        }
    }
    .apply_vip{
        background-color: #fff;
        padding-left: .7rem;
        margin-bottom: 0.5rem;
        .apply_vip_buy{
            @include fj;
            align-items: center;
            padding-right: .7rem;
            font-size: .7rem;
            line-height: 2.6rem;
            .apply_vip_buy_left{
                span:nth-of-type(2){
                    font-weight: bold;
                    color: #f60;
                }
            }
            .apply_vip_buy_right{
                border: 0.025rem solid #f60;
                border-radius: 0.2rem;
                line-height: 1.2rem;
                height: 1.2rem;
                width: 2.6rem;
                text-align: center;
                @include sc(.6rem, #f60);
            }
        }
    }
    .header_style{
        @include fj;
        background-color: #fff;
        line-height: 2rem;
        border-bottom: 1px solid #f5f5f5;
        padding-right: .7rem;
        .header_left{
            @include sc(.7rem, #333);
        }
        .header_right{
            display: flex;
            align-items: center;
            span{
                @include sc(.6rem, #999);
                margin-right: .2rem;
            }
            svg{
                @include wh(.5rem, .5rem);
            }
        }
    }
    .common_style{
        padding: 0 .7rem;
        margin-bottom: 0.5rem;
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translate3d(2rem, 0, 0);
        opacity: 0;
    }
</style>
