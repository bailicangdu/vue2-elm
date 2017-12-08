<template>
    <div class="confirmOrderContainer">
        <section v-if="!showLoading">
            <head-top head-title="确认订单" goBack="true" signin-up='confirmOrder'></head-top>
            <router-link :to='{path: "/confirmOrder/chooseAddress", query: {id: checkoutData.cart.id, sig: checkoutData.sig}}' class="address_container">
                <div class="address_empty_left">
                    <svg class="location_icon">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#location"></use>
                    </svg>
                    <div class="add_address" v-if="!choosedAddress">请添加一个收获地址</div>
                    <div v-else class="address_detail_container">
                        <header>
                            <span>{{choosedAddress.name}}</span>
                            <span>{{choosedAddress.sex == 1? '先生':'女士'}}</span>
                            <span>{{choosedAddress.phone}}</span>
                        </header>
                        <div class="address_detail">
                            <span v-if="choosedAddress.tag" :style="{backgroundColor: iconColor(choosedAddress.tag)}">{{choosedAddress.tag}}</span>
                            <p>{{choosedAddress.address_detail}}</p>
                        </div>
                    </div>
                </div>
                <svg class="address_empty_right">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                </svg>
            </router-link>
            <section class="delivery_model container_style">
                <p class="deliver_text">送达时间</p>
                <section class="deliver_time">
                    <p>尽快送达 | 预计 {{checkoutData.delivery_reach_time}}</p>
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
                <header v-if="checkoutData.cart.restaurant_info">
                    <img :src="imgBaseUrl + checkoutData.cart.restaurant_info.image_path">
                    <span>{{checkoutData.cart.restaurant_info.name}}</span>
                </header>
                <ul class="food_list_ul" v-if="checkoutData.cart.groups">
                    <li v-for="item in checkoutData.cart.groups[0]" :key="item.id" class="food_item_style">
                        <p class="food_name ellipsis">{{item.name}}</p>
                        <div class="num_price">
                            <span>x {{item.quantity}}</span>
                            <span>¥{{item.price}}</span>
                        </div>
                    </li>
                </ul>
                <div class="food_item_style" v-if="checkoutData.cart.extra">
                    <p class="food_name ellipsis">{{checkoutData.cart.extra[0].name}}</p>
                    <div class="num_price">
                        <span></span>
                        <span>¥ {{checkoutData.cart.extra[0].price}}</span>
                    </div>
                </div>
                <div class="food_item_style">
                    <p class="food_name ellipsis">配送费</p>
                    <div class="num_price">
                        <span></span>
                        <span>¥ {{checkoutData.cart.deliver_amount || 0}}</span>
                    </div>
                </div>
                <div class="food_item_style total_price">
                    <p class="food_name ellipsis">订单 ¥{{checkoutData.cart.total}}</p>
                    <div class="num_price">
                        <span>待支付 ¥{{checkoutData.cart.total}}</span>
                    </div>
                </div>
            </section>
            <section class="pay_way container_style">
                <router-link :to='{path: "/confirmOrder/remark", query: {id: checkoutData.cart.id, sig: checkoutData.sig}}' class="header_style">
                    <span>订单备注</span>
                    <div class="more_type">
                        <span class="ellipsis">{{remarkText||inputText? remarklist: '口味、偏好等'}}</span>
                        <svg class="address_empty_right">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </div>
                </router-link>
                <router-link :to="checkoutData.invoice.is_available? '/confirmOrder/invoice': ''" class="hongbo" :class="{support_is_available: checkoutData.invoice.is_available}">
                    <span>发票抬头</span>
                    <span>
                        {{checkoutData.invoice.status_text}}
                        <svg class="address_empty_right">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </router-link>
            </section>
            <section class="confrim_order">
                <p>待支付 ¥{{checkoutData.cart.total}}</p>
                <p @click="confrimOrder">确认下单</p>
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
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
        <transition name="router-slid" mode="out-in">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import headTop from 'src/components/header/head'
    import alertTip from 'src/components/common/alertTip'
    import loading from 'src/components/common/loading'
    import {checkout, getAddress, placeOrders, getAddressList} from 'src/service/getData'
    import {imgBaseUrl} from 'src/config/env'

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
                payWayId: 1, //付款方式
                showAlert: false, //弹出框
                alertText: null, //弹出框内容
            }
        },
        created(){
            //获取上个页面传递过来的geohash值
            this.geohash = this.$route.query.geohash;
            //获取上个页面传递过来的shopid值
            this.shopId = this.$route.query.shopId;
            this.INIT_BUYCART();
            this.SAVE_SHOPID(this.shopId);
            //获取当前商铺购物车信息
            this.shopCart = this.cartList[this.shopId];
        },
        mounted(){
            if (this.geohash) {
                this.initData();
                this.SAVE_GEOHASH(this.geohash);
            }
            if (!(this.userInfo && this.userInfo.user_id)) {
                // this.showAlert = true;
                // this.alertText = '您还没有登录';
            }
        },
        components: {
            headTop,
            alertTip,
            loading,
        },
        computed: {
            ...mapState([
                'cartList', 'remarkText', 'inputText', 'invoice', 'choosedAddress', 'userInfo'
            ]),
            //备注页返回的信息进行处理
            remarklist: function (){
                let str = new String;
                if (this.remarkText) {
                    Object.values(this.remarkText).forEach(item => {
                        str += item[1] + '，';
                    })
                }
                //是否有自定义备注，分开处理
                if (this.inputText) {
                    return str + this.inputText;
                }else{
                    return str.substr(0, str.lastIndexOf('，')) ;
                }
            },
        },
        methods: {
            ...mapMutations([
                'INIT_BUYCART', 'SAVE_GEOHASH', 'CHOOSE_ADDRESS', 'NEED_VALIDATION', 'SAVE_CART_ID_SIG', 'SAVE_ORDER_PARAM', 'ORDER_SUCCESS', 'SAVE_SHOPID'
            ]),
            //初始化数据
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
                //检验订单是否满足条件
                this.checkoutData = await checkout(this.geohash, [newArr], this.shopId);
                this.SAVE_CART_ID_SIG({cart_id: this.checkoutData.cart.id, sig:  this.checkoutData.sig})
                this.initAddress();
                this.showLoading = false;
            },
            //获取地址信息，第一个地址为默认选择地址
            async initAddress(){
                if (this.userInfo && this.userInfo.user_id) {
                    const addressRes = await getAddressList(this.userInfo.user_id);
                    if (addressRes instanceof Array && addressRes.length) {
                        this.CHOOSE_ADDRESS({address: addressRes[0], index: 0});
                    }
                }
            },
            //显示付款方式
            showPayWayFun(){
                this.showPayWay = !this.showPayWay;
            },
            //选择付款方式
            choosePayWay(is_online_payment, id){
                if (is_online_payment) {
                    this.showPayWay = !this.showPayWay;
                    this.payWayId = id;
                }
            },
            //地址备注颜色
            iconColor(name){
                switch(name){
                    case '公司': return '#4cd964';
                    case '学校': return '#3190e8';
                }
            },
            //确认订单
            async confrimOrder(){
                //用户未登录时弹出提示框
                if (!(this.userInfo && this.userInfo.user_id)) {
                    this.showAlert = true;
                    this.alertText = '请登录';
                    return
                    //未选择地址则提示
                }else if(!this.choosedAddress){
                    this.showAlert = true;
                    this.alertText = '请添加一个收获地址';
                    return
                }
                //保存订单
                this.SAVE_ORDER_PARAM({
                    user_id: this.userInfo.user_id,
                    cart_id: this.checkoutData.cart.id,
                    address_id: this.choosedAddress.id,
                    description: this.remarklist,
                    entities: this.checkoutData.cart.groups,
                    geohash: this.geohash,
                    sig: this.checkoutData.sig,
                });
                //发送订单信息
                let orderRes = await placeOrders(this.userInfo.user_id, this.checkoutData.cart.id, this.choosedAddress.id, this.remarklist, this.checkoutData.cart.groups, this.geohash, this.checkoutData.sig);
                //第一次下单的手机号需要进行验证，否则直接下单成功
                if (orderRes.need_validation) {
                    this.NEED_VALIDATION(orderRes);
                    this.$router.push('/confirmOrder/userValidation');
                }else{
                    this.ORDER_SUCCESS(orderRes);
                    this.$router.push('/confirmOrder/payment');
                }
            },
        },
        watch: {
            userInfo: function (value) {
                if (value && value.user_id) {
                    this.initAddress();
                }
            },
        }
    }

</script>

<style lang="scss" scoped>
    @import 'src/style/mixin';

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
            .address_detail_container{
                margin-left: .2rem;
                header{
                    @include sc(.65rem, #333);
                    span:nth-of-type(1){
                        font-size: .8rem;
                        font-weight: bold;
                    }
                }
                .address_detail{
                    width: 100%;
                    display: flex;
                    align-items: center;
                    span{
                        @include sc(.5rem, #fff);
                        border-radius: .15rem;
                        background-color: #ff5722;
                        height: .6rem;
                        line-height: .6rem;
                        padding: 0 .2rem;
                        margin-right: .3rem;
                    }
                    p{
                        @include sc(.55rem, #777);
                    }
                }
            }
        }
    }
    .address_empty_right{
        @include wh(.6rem, .6rem);
        fill: #999;
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
                    width: 10rem;
                    display: inline-block;
                    text-align: right;
                    vertical-align: middle;
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
                @include sc(.6rem, #aaa);
                line-height: 2rem;
                svg{
                    @include wh(.5rem, .5rem);
                    vertical-align: middle;
                    fill: #ccc;
                }
            }
            span:nth-of-type(2){
                color: #aaa;
            }
        }
        .support_is_available{
            span{
                color: #666;
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
                align-items: center;
                span:nth-of-type(1){
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
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translate3d(2rem, 0, 0);
        opacity: 0;
    }
</style>
