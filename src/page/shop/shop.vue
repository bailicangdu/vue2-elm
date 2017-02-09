 <template>
    <div class="shop_container">
        <header class="shop_detail_header" v-if="!showLoading" ref="shopheader" :style="{zIndex: showActivities? '14':'10'}">
            <img :src="getImgPath(shopDetailData.image_path)" class="header_cover_img">
            <section class="description_header">
                <section class="description_top" @click="showActivitiesFun">
                    <section class="description_left">
                        <img :src="getImgPath(shopDetailData.image_path)">
                    </section>
                    <section class="description_right">
                        <h4 class="description_title ellipsis">{{shopDetailData.name}}</h4>
                        <p class="description_text">商家配送／{{shopDetailData.order_lead_time}}分钟送达／配送费¥{{shopDetailData.float_delivery_fee}}</p>
                        <p class="description_promotion ellipsis">公告：{{promotionInfo}}</p>
                    </section>
                    <svg width="14" height="14" xmlns="http://www.w3.org/2000/svg" version="1.1" class="description_arrow" >
                        <path d="M0 0 L8 7 L0 14"  stroke="#fff" stroke-width="1" fill="none"/>
                    </svg>
                </section>
                <footer class="description_footer" v-if="shopDetailData.activities.length" @click="showActivitiesFun">
                    <p class="ellipsis">
                        <span class="tip_icon" :style="{backgroundColor: '#' + shopDetailData.activities[0].icon_color, borderColor: '#' + shopDetailData.activities[0].icon_color}">{{shopDetailData.activities[0].icon_name}}</span>
                        <span>{{shopDetailData.activities[0].description}}（APP专享）</span>
                    </p>
                    <p>{{shopDetailData.activities.length}}个活动</p>
                </footer>
                <transition name="fade">
                    <section class="activities_details" v-if="showActivities">
                        <h2 class="activities_shoptitle">{{shopDetailData.name}}</h2>
                        <h3 class="activities_ratingstar">
                            <rating-star :rating='shopDetailData.rating'></rating-star>
                        </h3>
                        <section class="activities_list">
                            <header class="activities_title_style"><span>优惠信息</span></header>
                            <ul>
                                <li v-for="item in shopDetailData.activities" :key="item.id">
                                    <span class="activities_icon" :style="{backgroundColor: '#' + item.icon_color, borderColor: '#' + item.icon_color}">{{item.icon_name}}</span>
                                    <span>{{item.description}}（APP专享）</span>
                                </li>
                            </ul>
                        </section>
                        <section class="activities_shopinfo">
                            <header class="activities_title_style"><span>商家公告</span></header>
                            <p>{{promotionInfo}}</p>
                        </section>
                        <svg width="60" height="60" class="close_activities" @click.stop="showActivitiesFun">
                            <circle cx="30" cy="30" r="25" stroke="#555" stroke-width="1" fill="none"/>
                            <line x1="22" y1="38" x2="38" y2="22" style="stroke:#999;stroke-width:2"/>
                            <line x1="22" y1="22" x2="38" y2="38" style="stroke:#999;stroke-width:2"/>
                        </svg>
                    </section>
                </transition>  
            </section>
        </header>
        <section class="menu_container" v-if="!showLoading">
            <ul class="menu_left">
                <li v-for="(item,index) in menuList" :key="index" class="menu_left_li" :class="{activity_menu: index == menuIndex}" @click="chooseMenu(index)">
                    <img :src="getImgPath(item.icon_url)" v-if="item.icon_url">
                    <span>{{item.name}}</span>
                    <span class="category_num" v-if="categoryNum[index]&&item.type==1">{{categoryNum[index]}}</span>
                </li>
            </ul>
            <ul class="menu_right" ref="menuFoodList">
                <li v-for="(item,index) in menuList" :key="index">
                    <header class="menu_detail_header">
                        <section class="menu_detail_header_left">
                            <strong class="menu_item_title">{{item.name}}</strong>
                            <span class="menu_item_description">{{item.description}}</span>
                        </section>
                        <span class="menu_detail_header_right" @click="showTitleDetail(index)"></span>
                        <p class="description_tip" v-if="index == TitleDetailIndex">
                            <span>{{item.name}}</span>
                            {{item.description}}
                        </p>
                    </header>
                    <section v-for="(foods,foodindex) in item.foods" :key="foodindex" class="menu_detail_list">
                        <router-link to="/shop/rating" tag="div" class="menu_detail_link">
                            <section class="menu_food_img">
                                <img :src="getImgPath(foods.image_path)">
                            </section>
                            <section class="menu_food_description">
                                <h3 class="food_description_head">
                                    <strong class="description_foodname">{{foods.name}}</strong>
                                    <ul v-if="foods.attributes.length" class="attributes_ul">
                                        <li v-for="(attribute, foodindex) in foods.attributes" :key="foodindex" :style="{color: '#' + attribute.icon_color,borderColor:'#' +attribute.icon_color}" :class="{attribute_new: attribute.icon_name == '新'}">
                                        <p :style="{color: attribute.icon_name == '新'? '#fff' : '#' + attribute.icon_color}">{{attribute.icon_name == '新'? '新品':attribute.icon_name}}</p>
                                        </li>
                                    </ul>
                                    
                                </h3>
                                <p class="food_description_content">{{foods.description}}</p>
                                <p class="food_description_sale_rating">
                                    <span>月售{{foods.month_sales}}份</span>
                                    <span>好评率{{foods.satisfy_rate}}%</span>
                                </p>
                                <p v-if="foods.activity" class="food_activity">
                                <span :style="{color: '#' + foods.activity.image_text_color,borderColor:'#' +foods.activity.icon_color}">{{foods.activity.image_text}}</span>
                                </p>
                            </section>
                        </router-link>
                        <footer class="menu_detail_footer">
                            <section class="food_price">
                                <span>¥</span>
                                <span>{{foods.specfoods[0].price}}</span>
                                <span v-if="foods.specifications.length">起</span>
                            </section>
                            <buy-cart :foods='foods' :shopCart='shopCart' @add="addToCart" @reduce="removeOutCart" @moveInCart="listenInCart"></buy-cart>
                        </footer>
                    </section>
                </li>
            </ul>
        </section>
        <section class="buy_cart_container">
            <section @click="toggleCartList" class="cart_icon_num">
                <div class="cart_icon_container" :class="{cart_icon_activity: totalPrice > 0, move_in_cart:receiveInCart}" ref="cartContainer">
                    <svg class="cart_icon">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-icon"></use>
                    </svg>
                </div>
                <div class="cart_num">
                    <div>¥ {{totalPrice}}</div>
                    <div>配送费¥{{deliveryFee}}</div>
                </div>
            </section>
            <section class="gotopay" :class="{gotopay_acitvity: minimumOrderAmount < 0}">
                <span class="gotopay_button_style" v-if="minimumOrderAmount > 0">还差¥{{minimumOrderAmount}}起送</span>
                <span class="gotopay_button_style" v-else >去结算</span>
            </section>
        </section>
        <transition name="toggle-cart">
            <section class="cart_food_list" v-show="showCartList&&cartFoodList.length">
                <header>
                    <h4>购物车</h4>
                    <div @click="clearCart">
                        <svg>
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-remove"></use>
                        </svg>
                        <span class="clear_cart">清空</span>
                    </div>
                </header>
                <section class="cart_food_details">
                    <ul>
                        <li v-for="(item, index) in cartFoodList" :key="index" class="cart_food_li">
                            <div class="cart_list_num">
                                <p class="ellipsis">{{item.name}}</p>
                                <p class="ellipsis">{{item.specs}}</p>
                            </div>
                            <div class="cart_list_price">
                                <span>¥</span>
                                <span>{{item.price}}</span>
                            </div>
                            <section class="cart_list_control">
                                <svg @click="removeOutCart(item.category_id, item.item_id, item.food_id, item.name, item.price, item.specs)">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                                </svg>
                                <span class="cart_num">{{item.num}}</span>
                                <svg class="cart_add" @click="addToCart(item.category_id, item.item_id, item.food_id, item.name, item.price, item.specs)">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-add"></use>
                                </svg>
                            </section>
                        </li>
                    </ul>
                </section>
            </section>
        </transition>
        <transition name="fade">
            <div class="screen_cover" v-show="showCartList&&cartFoodList.length" @click="toggleCartList"></div>
        </transition>
       <loading v-if="showLoading"></loading>
       <transition name="router-slid">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import {imgBaseUrl} from '../../config/env'
    import {msiteAdress, shopDetails, foodMenu} from '../../service/getData'
    import loading from '../../components/common/loading'
    import buyCart from '../../components/common/buyCart'
    import ratingStar from '../../components/common/ratingStar'
    import {getImgPath} from '../../components/common/mixin'
    import {animate} from '../../config/mUtils'

    export default {
        data(){
            return{
                geohash: '', //geohash位置信息
                shopId: null, //商店id值
                showLoading: true, //显示加载动画
                shopDetailData: null, //商铺详情
                showActivities: false, //是否显示活动详情
                menuList: [], //食品列表
                menuIndex: 0, //已选菜单索引值，默认为0
                menuIndexChange: true,//解决选中index时，scroll监听事件重复判断设置index的bug
                shopListTop: [], //商品列表的高度集合
                TitleDetailIndex: null, //点击展示列表头部详情
                categoryNum: [], //商品类型右上角已加入购物车的数量
                totalPrice: 0, //总共价格
                cartFoodList: [], //购物车商品列表
                showCartList: false,//显示购物车列表
                receiveInCart: false, //购物车组件下落的圆点是否到达目标位置
            }
        },
        created(){
            //获取上个页面传递过来的geohash值
            this.geohash = this.$route.query.geohash;
            //获取上个页面传递过来的shopid值
            this.shopId = this.$route.query.id;
            //初始化购物车，获取存储在localStorage中的购物车商品信息
            this.INIT_BUYCART();
        },
        mounted(){
            //初始化数据
            this.initData();
        },
        mixins: [getImgPath],
        components: {
            loading,
            ratingStar,
            buyCart,
        },
        computed: {
            ...mapState([
                'latitude','longitude','cartList'
            ]),
            //商铺公告
            promotionInfo: function (){ 
                return this.shopDetailData.promotion_info || '欢迎光临，用餐高峰期请提前下单，谢谢。'
            },
            //配送费
            deliveryFee: function () { 
                if (this.shopDetailData) {
                    return this.shopDetailData.float_delivery_fee;
                }else{
                    return null;
                }
            },
            //还差多少元起送，为负数时显示去结算按钮
            minimumOrderAmount: function () { 
                if (this.shopDetailData) {
                    return this.shopDetailData.float_minimum_order_amount - this.totalPrice;
                }else{
                    return null;
                }
            },
            /**
             * 监听cartList变化，更新当前商铺的购物车信息shopCart，同时返回一个新的对象，因为组件buyCart需要监听shopCart的变化
             */
            shopCart: function (){
                return Object.assign({},this.cartList[this.shopId]);
            }
        },
        methods: {
            ...mapMutations([
                'RECORD_ADDRESS','ADD_CART','REDUCE_CART','INIT_BUYCART','CLEAR_CART'
            ]),
            //初始化时获取基本数据
            async initData(){
                if (!this.latitude) {
                    //获取位置信息
                    let res = await msiteAdress(this.geohash);
                    // 记录当前经度纬度进入vuex
                    this.RECORD_ADDRESS(res);
                }
                //获取商铺信息
                this.shopDetailData = await shopDetails(this.shopId, this.latitude, this.longitude);
                //获取商铺食品列表
                this.menuList = await foodMenu(this.shopId);
                //隐藏加载动画
                this.showLoading = false;
            },
            //获取食品列表的高度，存入shopListTop
            getFoodListHeight(){
                const baseHeight = this.$refs.shopheader.clientHeight;
                const listContainer = this.$refs.menuFoodList;
                const listArr = Array.from(listContainer.children);
                listArr.forEach((item, index) => {
                    this.shopListTop[index] = item.offsetTop - baseHeight;
                });
                this.listenScroll(listContainer)
            },
            //当滑动食品列表时，监听其scrollTop值来设置对应的食品列表标题的样式
            listenScroll(element){
                let oldScrollTop;
                let requestFram;
                element.addEventListener('scroll',() => {
                   currenIndex();
                }, false)
                //运动过程中保持监听 scrollTop 的值
                element.addEventListener('touchmove',() => {
                   currenIndex();
                })
                //运动结束时判断是否有惯性运动
                element.addEventListener('touchend',() => {
                    oldScrollTop = element.scrollTop;
                    bounceMove();
                })
                //惯性运动进行和结束时判断是否有满足条件情况
                const bounceMove = () => {
                    requestFram = requestAnimationFrame(() => {
                        if (element.scrollTop != oldScrollTop) {
                            oldScrollTop = element.scrollTop;
                            currenIndex();
                            bounceMove();
                        }else{
                            cancelAnimationFrame(requestFram);
                            currenIndex();
                        }
                    })
                }
                //判断scrollTop的值，当前其值在shopListTop两个值中间位置，则满足条件，改变对应列表标题样式
                const currenIndex = () => {
                    this.shopListTop.forEach((item, index) => {
                        if (this.menuIndexChange && element.scrollTop  >= item && element.scrollTop < this.shopListTop[index + 1]) {
                            this.menuIndex = index;
                        }
                    })
                }
            },
            //控制活动详情页的显示隐藏
            showActivitiesFun(){
                this.showActivities = !this.showActivities;
            },
            //点击左侧食品列表标题，相应列表移动到最顶层
            chooseMenu(index){
                this.menuIndex = index;
                //menuIndexChange解决运动时listenScroll依然监听的bug
                this.menuIndexChange = false;
                animate(this.$refs.menuFoodList, {scrollTop: this.shopListTop[index]}, () => {
                    this.menuIndexChange = true;
                });
            },
            //控制显示列表标题详情提示
            showTitleDetail(index){
                if (this.TitleDetailIndex == index) {
                    this.TitleDetailIndex = null;
                }else{
                    this.TitleDetailIndex = index;
                }
            },
            //加入购物车，所需7个参数，商铺id，食品分类id，食品id，食品规格id，食品名字，食品价格，食品规格
            addToCart(category_id, item_id, food_id, name, price, specs){
                this.ADD_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs});
            },
            //移出购物车，所需7个参数，商铺id，食品分类id，食品id，食品规格id，食品名字，食品价格，食品规格
            removeOutCart(category_id, item_id, food_id, name, price, specs){
                this.REDUCE_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs});
            },
            /**
             * 初始化和shopCart变化时，重新获取购物车改变过的数据，赋值 categoryNum，totalPrice，cartFoodList，整个数据流是自上而下的形式，所有的购物车数据都交给vuex统一管理，包括购物车组件中自身的商品数量，使整个数据流更加清晰
             */
            initCategoryNum(){
                //左侧食品列表当前分类中已加入购物车的商品数量
                let newArr = [];
                let cartFoodNum = 0;
                //购物车总共的价格
                this.totalPrice = 0; 
                //购物车中所有商品的详细信息列表
                this.cartFoodList = [];
                this.menuList.forEach((item, index) => {
                    if (this.shopCart&&this.shopCart[item.foods[0].category_id]) {
                        let num = 0;
                        Object.keys(this.shopCart[item.foods[0].category_id]).forEach(itemid => {
                            Object.keys(this.shopCart[item.foods[0].category_id][itemid]).forEach(foodid => {
                                let foodItem = this.shopCart[item.foods[0].category_id][itemid][foodid];
                                num += foodItem.num;
                                if (item.type == 1) {
                                    this.totalPrice += foodItem.num*foodItem.price;
                                    if (foodItem.num > 0) {
                                        this.cartFoodList[cartFoodNum] = {};
                                        this.cartFoodList[cartFoodNum].category_id = item.foods[0].category_id;
                                        this.cartFoodList[cartFoodNum].item_id = itemid;
                                        this.cartFoodList[cartFoodNum].food_id = foodid;
                                        this.cartFoodList[cartFoodNum].num = foodItem.num;
                                        this.cartFoodList[cartFoodNum].price = foodItem.price;
                                        this.cartFoodList[cartFoodNum].name = foodItem.name;
                                        this.cartFoodList[cartFoodNum].specs = foodItem.specs;
                                        cartFoodNum ++;
                                    }
                                }
                            })
                        })
                        newArr[index] = num;
                    }else{
                        newArr[index] = 0;
                    }
                })
                this.categoryNum = newArr.concat([]);
            },
            //控制显示购物车中已选商品列表
            toggleCartList(){
                this.showCartList = !this.showCartList;
            },
            //清空当前商铺的购物车信息
            clearCart(){
                this.toggleCartList();
                this.CLEAR_CART(this.shopId);
            },
            //监听购物车组件下落的圆点，控制购物车图标进行运动效果
            listenInCart(){
                if (!this.receiveInCart) {
                    this.receiveInCart = true;
                    this.$refs.cartContainer.addEventListener('animationend', () => {
                        this.receiveInCart = false;
                    })
                }
            }
        },
        watch: {
            //showLoading变化时说明组件已经获取初始化数据，在下一帧nextTick进行后续操作
            showLoading: function (value){
                if (!value) {
                    this.$nextTick(() => {
                        this.getFoodListHeight();
                        this.initCategoryNum();
                    })
                }
            },
            //监听shopCart的变化
            shopCart: function (value){
                this.initCategoryNum();
            },
            //监听购物车中商铺列表的变化，当length为0时将列表隐藏
            cartFoodList: function (value){
                if(!value.length){
                    this.showCartList = false;
                }
            }
        }
    }
</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';
    @keyframes mymove{
       0%   { transform: scale(1) }
       25%  { transform: scale(.8) }
       50%  { transform: scale(1.1) }
       75%  { transform: scale(.9) }
       100% { transform: scale(1) }
    }
    .buy_cart_container{
        position: absolute;
        background-color: #3d3d3f;
        bottom: 0;
        left: 0;
        z-index: 13;
        display: flex;
        @include wh(100%, 2rem);
        .cart_icon_num{
            flex: 1;
            .cart_icon_container{
                background-color: #3d3d3f;
                position: absolute;
                padding: .4rem;
                border: 0.25rem solid #444;
                border-radius: 50%;
                left: .5rem;
                top: -.7rem;
                .cart_icon{
                    @include wh(1.2rem, 1.2rem);
                }
            }
            .move_in_cart{
                animation: mymove .5s ease-in-out;
            }
            .cart_icon_activity{
                 background-color: #3190e8;
            }
            .cart_num{
                @include ct;
                left: 3.5rem;

                div{
                    color: #fff;
                }
                div:nth-of-type(1){
                    font-size: .8rem;
                    font-weight: bold;
                    margin-bottom: .1rem;
                }
                div:nth-of-type(2){
                    font-size: .4rem;   
                }
            }
        }
        .gotopay{
            position: absolute;
            right: 0;
            background-color: #535356;
            @include wh(5rem, 100%);
            text-align: center;
            display: flex;
            align-items: center;
            justify-content: center;
            .gotopay_button_style{
                @include sc(.7rem, #fff);
                font-weight: bold;
            }
        }
        .gotopay_acitvity{
            background-color: #4cd964;
        }
    }
    .cart_food_list{
        position: fixed;
        width: 100%;
        padding-bottom: 2rem;
        z-index: 12;
        bottom: 0;
        left: 0;
        background-color: #fff;
        header{
            @include fj;
            align-items: center;
            padding: .3rem .6rem;
            background-color: #eceff1;
            svg{
                @include wh(.6rem,.6rem);
                vertical-align: middle;
            }
            h4{
                @include sc(.7rem, #666);
            }
            .clear_cart{
                @include sc(.6rem, #666);
            }
        }
        .cart_food_details{
            background-color: #fff;
            .cart_food_li{
                @include fj;
                padding: .6rem .5rem;
                .cart_list_num{
                    width: 55%;
                    p:nth-of-type(1){
                        @include sc(.7rem, #666);
                        font-weight: bold;
                    }
                    p:nth-of-type(2){
                        @include sc(.4rem, #666);
                    }
                }
                .cart_list_price{
                    font-size: 0;
                    span:nth-of-type(1){
                        @include sc(.6rem, #f60);
                        font-family: Helvetica Neue,Tahoma;

                    }
                    span:nth-of-type(2){
                        @include sc(.7rem, #f60);
                        font-family: Helvetica Neue,Tahoma;
                        font-weight: bold;
                    }
                }
                .cart_list_control{
                    display: flex;
                    align-items: center;
                    svg{
                        @include wh(.8rem, .8rem);
                        fill: #3190e8;
                    }
                    .specs_reduce_icon{
                        fill: #999;
                    }
                    .cart_num{
                        @include sc(.6rem, #666);
                        min-width: 1rem;
                        text-align: center;
                        font-family: Helvetica Neue,Tahoma;
                    }
                }
            }
        }
    }
    .screen_cover{
        position: fixed;
        top: 0;
        bottom: 0;
        left: 0;
        right: 0;
        background-color: rgba(0,0,0,.3);
        z-index: 11;
    }




    .shop_container{
        display: flex;
        flex-direction: column;
        position: absolute;
        right: 0;
        left: 0;
        height: 100%;
        overflow: hidden;
        padding-bottom: 2rem;
    }
    
    .shop_detail_header{
        overflow: hidden;
        position: relative;
        .header_cover_img{
            width: 100%;
            position: absolute;
            top: 0;
            left: 0;
            z-index: 9;
            filter: blur(10px);
        }
        .description_header{
            position: relative;
            z-index: 10;
            background-color: rgba(119,103,137,.43);
            padding: 0.4rem 0 0.4rem 0.4rem;
            width: 100%;
            overflow: hidden;
            .description_top{
                display: flex;
                .description_left{
                    margin-right: 0.3rem;
                    img{
                        @include wh(2.9rem, 2.9rem);
                        display: block;
                        border-radius: 0.15rem;
                    }
                }
                .description_right{
                    flex: 1;
                    .description_title{
                        @include sc(.8rem, #fff);
                        font-weight: bold;
                        width: 100%;
                        margin-bottom: 0.3rem;
                    }
                    .description_text{
                        @include sc(.5rem, #fff);
                        margin-bottom: 0.3rem;
                    }
                    .description_promotion{
                        @include sc(.5rem, #fff);
                        width: 12.5rem;
                    }
                }
                .description_arrow{
                    @include ct;
                    right: 0.3rem;
                    z-index: 11;
                }
            }
            .description_footer{
                @include fj;
                margin-top: 0.5rem;
                padding-right: .4rem;
                p{
                    @include sc(.5rem, #fff);
                    span{
                        color: #fff;
                    }
                    .tip_icon{
                        padding: .01rem;
                        border: 0.025rem solid #fff;
                        border-radius: 0.1rem;
                        display: inline-block;
                    }
                }
                .ellipsis{
                    width: 87%;
                }
            }
            .activities_details{
                position: fixed;
                top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background-color: #262626;
                z-index: 21;
                padding: 1.25rem;
                .activities_shoptitle{
                    text-align: center;
                    @include sc(.8rem, #fff);
                }
                .activities_ratingstar{
                    display: flex;
                    justify-content: center;
                    transform: scale(2.2);
                    margin-top: .7rem;
                }
                .activities_list{
                    margin-top: 1.5rem;
                    margin-bottom: 1rem;
                    @include sc(.5rem, #fff);
                    li{
                        margin-bottom: .2rem;
                        .activities_icon{
                            padding: .01rem;
                            border: 0.025rem solid #fff;
                            border-radius: 0.1rem;
                        }
                        span{
                            color: #fff;
                            line-height: .6rem;
                        }
                    }
                }
                .activities_shopinfo{
                    p{
                        line-height: .7rem;
                        @include sc(.5rem, #fff);
                    }
                }
                .activities_title_style{
                    text-align: center;
                    margin-bottom: 1rem;
                    span{
                        @include sc(.5rem, #fff);
                        border: 0.025rem solid #555;
                        padding: .2rem .4rem;
                        border-radius: 0.5rem;
                    }

                }
                .close_activities{
                    position: absolute;
                    bottom: 1rem;
                    @include cl;
                }
            }
            
            
        }
    }

    .menu_container{
        display: flex;
        flex: 1;
        overflow-y: auto;
        .menu_left{
            background-color: #f8f8f8;
            overflow-y: auto;
            width: 3.8rem;
            .menu_left_li{
                padding: .7rem .3rem;
                border-bottom: 0.025rem solid #ededed;
                box-sizing: border-box;
                border-left: 0.15rem solid #f8f8f8;
                position: relative;
                img{
                    @include wh(.5rem, .6rem);
                }
                span{
                    @include sc(.6rem, #666);
                }
                .category_num{
                    position: absolute;
                    top: .1rem;
                    right: .1rem;
                    background-color: #ff461d;
                    line-height: .6rem;
                    text-align: center;
                    border-radius: 50%;
                    border: 0.025rem solid #ff461d;
                    min-width: .6rem;
                    height: .6rem;
                    @include sc(.5rem, #fff);
                    font-family: Helvetica Neue,Tahoma,Arial;
                }
            }
            .activity_menu{
                border-left: 0.15rem solid #3190e8;
                background-color: #fff;
                span:nth-of-type(1){
                    font-weight: bold;
                }
            }
        }
        .menu_right{
            flex: 4;
            overflow-y: auto;
            .menu_detail_header{
                width: 100%;
                padding: .4rem;
                position: relative;
                @include fj;
                align-items: center;
                .menu_detail_header_left{
                    width: 11rem;
                    white-space: nowrap;
                    overflow: hidden;
                    .menu_item_title{
                        @include sc(.7rem, #666);
                        font-weight: bold;
                    }
                    .menu_item_description{
                        @include sc(.5rem, #999);
                        width: 30%;
                        overflow: hidden;
                    }
                }
                .menu_detail_header_right{
                    @include wh(.5rem, 1rem);
                    display: block;
                    @include bis('../../images/icon_point.png');
                    background-size: 100% .4rem;
                    background-position: left center;
                }
                .description_tip{
                    background-color: #39373a;
                    opacity: 0.95;
                    @include sc(.5rem, #fff);
                    position: absolute;
                    top: 1.5rem;
                    z-index: 14;
                    width: 8rem;
                    right: .2rem;
                    padding: .5rem .4rem;
                    border: 1px;
                    border-radius: .2rem;
                    span{
                        color: #fff;
                        line-height: .6rem;
                        font-size: .55rem;
                    }
                }
                .description_tip::after{
                    content: '';
                    position: absolute;
                    @include wh(.4rem, .4rem);
                    background-color: #39373a;
                    top: -.5rem;
                    right: .7rem;
                    transform: rotate(-45deg) translateY(.41rem);
                }
            }
            .menu_detail_list{
                background-color: #fff;
                padding: .6rem .4rem;
                border-bottom: 1px solid #f8f8f8;
                position: relative;
                overflow: hidden;            
                .menu_detail_link{
                    display:flex;
                    .menu_food_img{
                        margin-right: .4rem;
                        img{
                            @include wh(2rem, 2rem);
                            display: block;
                        }
                    }
                    .menu_food_description{
                        width: 100%;
                        .food_description_head{
                            @include fj;
                            margin-bottom: .2rem;
                            .description_foodname{
                                @include sc(.7rem, #333);
                            }
                            .attributes_ul{
                                display: flex;
                                li{
                                    font-size: .3rem;
                                    height: .6rem;
                                    line-height: .35rem;
                                    padding: .1rem;
                                    border: 1px solid #666;
                                    border-radius: 0.3rem;
                                    margin-right: .1rem;
                                    transform: scale(.8);
                                }
                                .attribute_new{
                                    position: absolute;
                                    top: 0;
                                    left: 0;
                                    background-color: #4cd964;
                                    @include wh(2rem, 2rem);
                                    display: flex;
                                    align-items: flex-end;
                                    transform: rotate(-45deg) translate(-.1rem, -1.5rem);
                                    border: none;
                                    border-radius: 0;
                                    p{
                                        @include sc(.4rem, #fff);
                                        text-align: center;
                                        flex: 1;
                                    }
                                }
                            }
                        }
                        .food_description_content{
                            @include sc(.5rem, #999);
                            line-height: .6rem;
                        }
                        .food_description_sale_rating{
                            line-height: .8rem;
                            span{
                                @include sc(.5rem, #333);
                            }
                        }
                        .food_activity{
                            line-height: .4rem;
                            span{
                                font-size: .3rem;
                                border: 1px solid currentColor;
                                border-radius: 0.3rem;
                                padding: .08rem;
                                display: inline-block;
                                transform: scale(.8);
                                margin-left: -0.35rem;

                            }
                        }
                    }
                }
                .menu_detail_footer{
                    margin-left: 2.4rem;
                    font-size: 0;
                    margin-top: .3rem;
                    @include fj;
                    .food_price{
                        span{
                            font-family: 'Helvetica Neue',Tahoma,Arial;
                        }
                        span:nth-of-type(1){
                            @include sc(.5rem, #f60);
                            margin-right: .05rem;
                        }
                        span:nth-of-type(2){
                            @include sc(.7rem, #f60);
                            font-weight: bold;
                            margin-right: .3rem;
                        }
                        span:nth-of-type(3){
                            @include sc(.5rem, #666);
                        }
                    }
                }
            }
        }
    }
    .fade-enter-active, .fade-leave-active {
        transition: opacity .5s;
    }
    .fade-enter, .fade-leave-active {
        opacity: 0;
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translateX(100%);
    }
    .toggle-cart-enter-active, .toggle-cart-leave-active {
        transition: all .3s ease-out;
    }
    .toggle-cart-enter, .toggle-cart-leave-active {
        transform: translateY(100%);
    }
</style>
