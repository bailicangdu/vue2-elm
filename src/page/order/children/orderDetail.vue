 <template>
  <div class="order_detail_page">
        <head-top head-title="订单详情" go-back='true'></head-top>
        <section v-if="!showLoading" id="scroll_section" class="scroll_container">
            <section class="scroll_insert">
                <section class="order_titel">
                    <img :src="imgBaseUrl + orderDetail.restaurant_image_url">
                    <p>{{orderDetail.status_bar.title}}</p>
                    <p>{{orderDetail.timeline_node.description}}</p>
                    <router-link class="order_again" :to="{path: '/shop', query: {geohash, id: orderDetail.restaurant_id}}">再来一单</router-link>
                </section>
                <section class="food_list">
                    <router-link class="food_list_header" :to="{path: '/shop', query: {geohash, id: orderDetail.restaurant_id}}">
                        <div class="shop_name">
                            <img :src="imgBaseUrl + orderDetail.restaurant_image_url">
                            <span>{{orderDetail.restaurant_name}}</span>
                        </div>
                        <svg fill="#333" class="arrow_right">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </router-link>
                    <ul class="food_list_ul">
                        <li v-for="item in orderDetail.basket.group[0]">
                            <p class="food_name ellipsis">{{item.name}}</p>
                            <div class="quantity_price">
                                <span>X{{item.quantity}}</span>
                                <span>¥{{item.price}}</span>
                            </div>
                        </li>
                    </ul>
                    <div class="deliver_fee">
                        <span>配送费</span>
                        <span>{{orderDetail.basket.deliver_fee&&orderDetail.basket.deliver_fee.price || 0}}</span>   
                    </div>
                    <div class="pay_ment">实付{{orderDetail.total_amount.toFixed(2)}}</div>
                </section>
                <section class="order_detail_style">
                    <header>配送信息</header>
                    <section class="item_style">
                        <p class="item_left">送达时间：</p>
                        <div class="item_right">
                            <p>{{orderData.deliver_time}}</p>
                        </div>
                    </section>
                    <section class="item_style">
                        <p class="item_left">送货地址：</p>
                        <div class="item_right">
                            <p>{{orderData.consignee}}</p>
                            <p>{{orderData.phone}}</p>
                            <p>{{orderData.address}}</p>
                        </div>
                    </section>
                    <section class="item_style">
                        <p class="item_left">配送方式：</p>
                        <div class="item_right">
                            <p>蜂鸟专送</p>
                        </div>
                    </section>
                </section>
                <section class="order_detail_style">
                    <header>订单信息</header>
                    <section class="item_style">
                        <p class="item_left">订单号：</p>
                        <div class="item_right">
                            <p>{{orderDetail.id}}</p>
                        </div>
                    </section>
                    <section class="item_style">
                        <p class="item_left">支付方式：</p>
                        <div class="item_right">
                            <p>在线支付</p>
                        </div>
                    </section>
                    <section class="item_style">
                        <p class="item_left">下单时间：</p>
                        <div class="item_right">
                            <p>{{orderDetail.formatted_created_at}}</p>
                        </div>
                    </section>
                </section>
            </section>
        </section>
        <transition name="loading">
            <loading v-if="showLoading"></loading>
        </transition>
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import headTop from 'src/components/header/head'
    import {getImgPath} from 'src/components/common/mixin'
    import {getOrderDetail} from 'src/service/getData'
    import loading from 'src/components/common/loading'
    import BScroll from 'better-scroll'
    import {imgBaseUrl} from 'src/config/env'


    export default {

      data(){
            return{
                showLoading: true, //显示加载动画
                orderData: null,
                imgBaseUrl
            }
        },
        mounted(){
            this.initData();
        },
        mixins: [getImgPath],
        components: {
            headTop,
            loading,
        },
        computed: {
            ...mapState([
                'orderDetail', 'geohash', 'userInfo'
            ]),
        },
        methods: {
            async initData(){
                if (this.userInfo && this.userInfo.user_id) {
                    this.orderData = await getOrderDetail(this.userInfo.user_id, this.orderDetail.unique_id);
                    this.showLoading = false;
                    this.$nextTick(() => {
                        new BScroll('#scroll_section', {  
                            deceleration: 0.001,
                            bounce: true,
                            swipeTime: 1800,
                            click: true,
                        }); 
                    })
                }
            },
        },
        watch: {
            userInfo: function (value) {
                if (value && value.user_id) {
                    this.initData();
                }
            }
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .order_detail_page{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #f1f1f1;
        z-index: 202;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .scroll_container{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        padding-top: 1.95rem;
    }
    .scroll_insert{
        padding-bottom: 3rem;
    }
    .order_titel{
        display: flex;
        flex-direction: column;
        align-items: center;
        padding: .7rem;
        background-color: #fff;
        margin-bottom: 0.5rem;
        img{
            border: 0.05rem solid $blue;
            border-radius: 50%;
            @include wh(3rem, 3rem);
        }
        p:nth-of-type(1){
            @include sc(.9rem, #333);
            font-weight: bold;
            margin-top: .4rem;
        }
        p:nth-of-type(2){
            @include sc(.55rem, #999);
            width: 10rem;
            margin-top: .3rem;
            text-align: center;
        }
        .order_again{
            @include sc(.6rem, $blue);
            margin-top: .5rem;
            border: 0.025rem solid $blue;
            padding: .15rem .4rem;
            border-radius: .1rem;
        }
    }
    .food_list{
        background-color: #fff;
        .food_list_header{
            @include fj;
            align-items: center;
            padding: .2rem .5rem;
            border-bottom: 1px solid #f5f5f5;
            .shop_name{
                img{
                    @include wh(1.2rem, 1.2rem);
                    vertical-align: middle;
                    margin-right: .2rem;
                }
                span{
                    @include sc(.75rem, #333);
                    font-weight: bold;
                }
            }
            svg{
                @include wh(.6rem, .6rem);
                fill: #666;
            }
        }
        .food_list_ul{
            li{
                @include fj;
                align-items: center;
                padding: 0 .5rem;
                line-height: 2rem;
                .food_name{
                    @include sc(.6rem, #666);
                    flex: 4;
                }
                .quantity_price{
                    flex: 1;
                    @include fj;
                    align-items: center;
                    span:nth-of-type(1){
                        @include sc(.6rem, #ccc);
                    }
                    span:nth-of-type(2){
                        @include sc(.6rem, #666);
                    }
                }
            }
        }
        .deliver_fee{
            @include fj;
            align-items: center;
            padding: 0 .5rem;
            line-height: 2rem;
            border-top: 1px solid #f5f5f5;
            span{
                @include sc(.6rem, #666);
            }
        }
        .pay_ment{
            @include sc(.6rem, #fb6b23);
            border-top: 1px solid #f5f5f5;
            font-weight: bold;
            line-height: 2rem;
            text-align: right;
            padding-right: .5rem;
        }
    }
    .order_detail_style{
        background-color: #fff;
        margin-top: 0.5rem;
        header{
            @include sc(.75rem, #333);
            padding: .5rem;
            border-bottom: 1px solid #f5f5f5;
        }
        .item_style{
            display: flex;
            padding: .5rem;
            p{
                @include sc(.65rem, #666);
                line-height: 1rem;
            }
        }
    }
    .loading-enter-active, .loading-leave-active {
        transition: opacity .7s
    }
    .loading-enter, .loading-leave-active {
        opacity: 0
    }
    
</style>
