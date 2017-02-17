<template>
    <div class="confirmOrderContainer">
        <section v-if="!showLoading">
            <head-top head-title="确认订单" goBack="true" signin-up='confirmOrder'></head-top>
            <section class="address_container">
                <div class="address_empty_left">
                    <svg class="location_icon">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#location"></use>
                    </svg>
                    <span class="add_address">请添加一个收获地址</span>
                </div>
                <svg class="address_empty_right">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                </svg>
            </section>
            <section class="delivery_model container_style">
                <p class="deliver_text">送达时间</p>
                <section class="deliver_time">
                    <p>尽快送达 | 预计{{checkoutData.delivery_reach_time}}</p>
                    <p v-if="checkoutData.cart.is_deliver_by_fengniao">蜂鸟专送</p>
                </section>
            </section>
            <section class="pay_way container_style">
                <header class="header_style">
                    <span>支付方式</span>
                    <div class="more_type" @click="showPayWayFun">
                        <span>在线支付</span>
                        <svg class="address_empty_right">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </div>
                </header>
                <section class="hongbo">
                    <span>红包</span>
                    <span>暂时只在饿了么 APP 中支持</span>
                </section>
            </section>
            <section class="food_list">
                <header>
                    <img :src="imgBaseUrl + checkoutData.cart.restaurant_info.image_path">
                    <span>{{checkoutData.cart.restaurant_info.name}}</span>
                </header>
                <ul class="food_list_ul">
                    <li v-for="item in checkoutData.cart.groups[0]" :key="item.id" class="food_item_style">
                        <p class="food_name ellipsis">{{item.name}}</p>
                        <div class="num_price">
                            <span>x {{item.quantity}}</span>
                            <span>¥{{item.price}}</span>
                        </div>
                    </li>
                </ul>
                <div class="food_item_style">
                    <p class="food_name ellipsis">{{checkoutData.cart.extra[0].name}}</p>
                    <div class="num_price">
                        <span></span>
                        <span>¥ {{checkoutData.cart.extra[0].price}}</span>
                    </div>
                </div>
                <div class="food_item_style total_price">
                    <p class="food_name ellipsis">订单 ¥{{checkoutData.cart.total}}</p>
                    <div class="num_price">
                        <span></span>
                        <span>待支付 ¥{{checkoutData.cart.total}}</span>
                    </div>
                </div>
            </section>
            <section class="pay_way container_style">
                <header class="header_style">
                    <span>订单备注</span>
                    <div class="more_type">
                        <span>口味偏、好等</span>
                        <svg class="address_empty_right">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </div>
                </header>
                <section class="hongbo">
                    <span>发票抬头</span>
                    <span>商家不支持开发票</span>
                </section>
            </section>
            <section class="confrim_order">
                <p>待支付 ¥{{checkoutData.cart.total}}</p>
                <p>确认下单</p>
            </section>
            <transition name="fade">
                <div class="cover" v-if="showPayWay" @click="showPayWayFun"></div>
            </transition>
            <transition name="slid_up">
                <div class="choose_type_Container" v-if="showPayWay">
                    <header>支付方式</header>
                    <ul>
                        <li v-for="item in checkoutData.payments" :key="item.id" :class="{choose: payWayId == item.id}">
                            <span>{{item.name}}<span v-if="!item.is_online_payment">{{item.description}}</span></span>
                            <svg class="address_empty_right" @click="choosePayWay(item.is_online_payment, item.id)">
                                <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#select"></use>
                            </svg>
                        </li>
                    </ul>
                </div>
            </transition>
        </section>
        <loading v-if="showLoading"></loading>    
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import headTop from '../../components/header/head'
    import alertTip from '../../components/common/alertTip'
    import loading from '../../components/common/loading'
    import {checkout} from '../../service/getData'
    import {imgBaseUrl} from '../../config/env'

    export default {
        data(){
            return {
                geohash: '', //geohash位置信息
                shopId: null, //商店id值
                showLoading: true, //显示加载动画
                checkoutData: null,//数据返回值
                shopCart: null,//购物车数据
                imgBaseUrl, //图片域名
                showPayWay: false,//显示付款方式
                payWayId: 1,
            }
        },
        created(){
            //获取上个页面传递过来的geohash值
            this.geohash = this.$route.query.geohash;
            //获取上个页面传递过来的shopid值
            this.shopId = this.$route.query.shopId;
            this.INIT_BUYCART();
            this.shopCart = this.cartList[this.shopId];
        },
        mounted(){
            this.initData();
        },
        components: {
            headTop,
            alertTip,
            loading,
        },
        computed: {
            ...mapState([
                'cartList'
            ]),
        },
        methods: {
            ...mapMutations([
                'INIT_BUYCART'
            ]),
            async initData(){
                let newArr = new Array;
                Object.values(this.shopCart).forEach(categoryItem => {
                    Object.values(categoryItem).forEach(itemValue=> {
                        Object.values(itemValue).forEach(item => {
                            newArr.push({
                                attrs: [],
                                extra: {},
                                id: item.id,
                                name: item.name,
                                packing_fee: item.packing_fee,
                                price: item.price,
                                quantity: item.num,
                                sku_id: item.sku_id,
                                specs: [item.specs],
                                stock: item.stock,
                            })
                        })
                    })
                })
                this.checkoutData = await checkout(this.geohash, [newArr]);
                this.showLoading = false;
            },
            showPayWayFun(){
                this.showPayWay = !this.showPayWay;
            },
            choosePayWay(is_online_payment, id){
                if (is_online_payment) {
                    this.showPayWay = !this.showPayWay;
                    this.payWayId = id;
                }
            },
        }
    }

</script>

<style lang="scss" scoped>
    @import '../../style/mixin';

    .confirmOrderContainer{
        padding-top: 1.95rem;
        padding-bottom: 3rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .container_style{
        background-color: #fff;
        margin-top: .4rem;
        padding: 0 .7rem;
    }
    .address_container{
        min-height: 3.5rem;
        @include fj;
        align-items: center;
        padding: 0 0.6rem;
        background: url(../../images/address_bottom.png) left bottom repeat-x;
        background-color: #fff;
        background-size: auto .12rem;
        .address_empty_left{
            display: flex;
            align-items: center;
            .location_icon{
                @include wh(.8rem, .8rem);
                fill: $blue;
                margin-right: .2rem;
            }
            .add_address{
                @include sc(.7rem, #333);
            }
        }
        .address_empty_right{
            @include wh(.6rem, .6rem);
            fill: #999;
        }
    }
    .delivery_model{
        border-left: .2rem solid $blue;
        min-height: 4rem;
        @include fj;
        align-items: center;
        .deliver_text{
            @include sc(.8rem, #333);
            font-weight: bold;
            padding-left: .3rem;
        }
        .deliver_time{
            display: flex;
            flex-direction: column;
            align-items: flex-end;
            p:nth-of-type(1){
                @include sc(.7rem, $blue);
            }
            p:nth-of-type(2){
                @include sc(.5rem, #fff);
                background-color: $blue;
                width: 2.4rem;
                margin-top: .5rem;
                text-align: center;
                border-radius: 0.12rem;
                padding: .1rem;
            }
        }
    }
    .pay_way{
        .header_style{
            @include fj;
            line-height: 2rem;
            span:nth-of-type(1){
                @include sc(.7rem, #666);
            }
            .more_type{
                span:nth-of-type(1){
                    @include sc(.6rem, #aaa);
                }
                svg{
                    @include wh(.5rem, .5rem);
                    fill: #ccc;
                }
            }
        }
        .hongbo{
            @include fj;
            border-top: 0.025rem solid #f5f5f5;
            span{
                @include sc(.6rem, #ccc);
                line-height: 2rem;
            }
        }
    }
    .food_list{
        background-color: #fff;
        margin-top: .4rem;
        header{
            padding: .7rem;
            border-bottom: 0.025rem solid #f5f5f5; 
            img{
                @include wh(1.2rem, 1.2rem);
                vertical-align: middle;
            }
            span{
                @include sc(.8rem, #333);
            }
        }
        .food_list_ul{
            padding-top: .5rem;
        }
        .food_item_style{
            @include fj;
            line-height: 1.8rem;
            padding: 0 .7rem;
            span,p{
                @include sc(.65rem, #666);
            }
            .food_name{
                width: 11rem;
            }
            .num_price{
                flex: 1;
                @include fj;
                span:nth-of-type(2){
                    color: #f60;
                }
            }
        }
        .total_price{
            border-top: 0.025rem solid #f5f5f5; 
        }
    }
    .confrim_order{
        display: flex;
        position: fixed;
        bottom: 0;
        width: 100%;
        height: 2rem;
        p{
            line-height: 2rem;
            @include sc(.75rem, #fff);
        }
        p:nth-of-type(1){
            background-color: #3c3c3c;
            flex: 5;
            padding-left: .7rem;
        }
        p:nth-of-type(2){
            flex: 2;
            background-color: #56d176;
            text-align: center;
        }
    }
    .cover{
        position: fixed;
        top: 0;
        bottom: 0;
        left: 0;
        right: 0;
        background-color: rgba(0,0,0,.3);
        z-index: 203;
    }
    .choose_type_Container{
        min-height: 10rem;
        background-color: #fff;
        position: fixed;
        bottom: 0;
        width: 100%;
        z-index: 204;
        header{
            background-color: #fafafa;
            @include sc(.7rem, #333);
            text-align: center;
            line-height: 2rem;
        }
        ul{
            li{
                @include fj;
                padding: 0 .7rem;
                line-height: 2.5rem;
                align-items: center;
                span{
                    @include sc(.7rem, #ccc);
                }
                svg{
                    @include wh(.8rem, .8rem);
                    fill: #eee;
                }
            }
            .choose{
                span{
                    color: #333;
                }
                svg{
                    fill: #4cd964;
                }
            }
        }
    }
    .fade-enter-active, .fade-leave-active {
        transition: opacity .3s;
    }
    .fade-enter, .fade-leave-active {
        opacity: 0;
    }
    .slid_up-enter-active, .slid_up-leave-active {
        transition: all .3s;
    }
    .slid_up-enter, .slid_up-leave-active {
        transform: translate3d(0,10rem,0)
    }
</style>
