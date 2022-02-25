 <template>
    <div>
        <section v-if="!showLoading" class="shop_container">
            <nav class="goback" @click="goback">
                <svg width="4rem" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1">
                    <polyline points="12,18 4,9 12,0" style="fill:none;stroke:rgb(255,255,255);stroke-width:3"/>
                </svg>
            </nav>
            <header class="shop_detail_header" ref="shopheader" :style="{zIndex: showActivities? '14':'10'}">
                <div class="header_cover_img_con">
                  <img :src="imgBaseUrl + shopDetailData.image_path" class="header_cover_img">
                </div>
                <section class="description_header">
                    <router-link to="/shop/shopDetail" class="description_top">
                        <section class="description_left">
                            <img :src="imgBaseUrl + shopDetailData.image_path">
                        </section>
                        <section class="description_right">
                            <h4 class="description_title ellipsis">{{shopDetailData.name}}</h4>
                            <p class="description_text">商家配送／{{shopDetailData.order_lead_time}}分钟送达／配送费¥{{shopDetailData.float_delivery_fee}}</p>
                            <p class="description_promotion ellipsis">公告：{{promotionInfo}}</p>
                        </section>
                        <svg width="14" height="14" xmlns="http://www.w3.org/2000/svg" version="1.1" class="description_arrow" >
                            <path d="M0 0 L8 7 L0 14"  stroke="#fff" stroke-width="1" fill="none"/>
                        </svg>
                    </router-link>
                    <footer class="description_footer" v-if="shopDetailData.activities.length" @click="showActivitiesFun">
                        <p class="ellipsis">
                            <span class="tip_icon" :style="{backgroundColor: '#' + shopDetailData.activities[0].icon_color, borderColor: '#' + shopDetailData.activities[0].icon_color}">{{shopDetailData.activities[0].icon_name}}</span>
                            <span>{{shopDetailData.activities[0].description}}（APP专享）</span>
                        </p>
                        <p>{{shopDetailData.activities.length}}个活动</p>
                        <svg class="footer_arrow">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-left"></use>
                        </svg>
                    </footer>

                </section>
            </header>
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
            <section class="change_show_type" ref="chooseType">
                <div>
                    <span :class='{activity_show: changeShowType =="food"}' @click="changeShowType='food'">商品</span>
                </div>
                <div>
                    <span :class='{activity_show: changeShowType =="rating"}' @click="changeShowType='rating'">评价</span>
                </div>
            </section>
            <transition name="fade-choose">
                <section v-show="changeShowType =='food'" class="food_container">
                    <section class="menu_container">
                        <section class="menu_left" id="wrapper_menu" ref="wrapperMenu">
                            <ul>
                                <li v-for="(item,index) in menuList" :key="index" class="menu_left_li" :class="{activity_menu: index == menuIndex}" @click="chooseMenu(index)">
                                    <img :src="getImgPath(item.icon_url)" v-if="item.icon_url">
                                    <span>{{item.name}}</span>
                                    <span class="category_num" v-if="categoryNum[index]&&item.type==1">{{categoryNum[index]}}</span>
                                </li>
                            </ul>
                        </section>
                        <section class="menu_right" ref="menuFoodList">
                            <ul>
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
                                        <router-link  :to="{path: 'shop/foodDetail', query:{image_path:foods.image_path, description: foods.description, month_sales: foods.month_sales, name: foods.name, rating: foods.rating, rating_count: foods.rating_count, satisfy_rate: foods.satisfy_rate, foods, shopId}}" tag="div" class="menu_detail_link">
                                            <section class="menu_food_img">
                                                <img :src="imgBaseUrl + foods.image_path">
                                            </section>
                                            <section class="menu_food_description">
                                                <h3 class="food_description_head">
                                                    <strong class="description_foodname">{{foods.name}}</strong>
                                                    <ul v-if="foods.attributes.length" class="attributes_ul">
                                                        <li v-if="attribute" v-for="(attribute, foodindex) in foods.attributes" :key="foodindex" :style="{color: '#' + attribute.icon_color,borderColor:'#' + attribute.icon_color}" :class="{attribute_new: attribute.icon_name == '新'}">
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
                                            <buy-cart :shopId='shopId' :foods='foods' @moveInCart="listenInCart" @showChooseList="showChooseList" @showReduceTip="showReduceTip" @showMoveDot="showMoveDotFun"></buy-cart>
                                        </footer>
                                    </section>
                                </li>
                            </ul>
                        </section>
                    </section>
                    <section class="buy_cart_container">
                        <section @click="toggleCartList" class="cart_icon_num">
                            <div class="cart_icon_container" :class="{cart_icon_activity: totalPrice > 0, move_in_cart:receiveInCart}" ref="cartContainer">
                                <span v-if="totalNum" class="cart_list_length">
                                    {{totalNum}}
                                </span>
                                <svg class="cart_icon">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-icon"></use>
                                </svg>
                            </div>
                            <div class="cart_num">
                                <div>¥ {{totalPrice}}</div>
                                <div>配送费¥{{deliveryFee}}</div>
                            </div>
                        </section>
                        <section class="gotopay" :class="{gotopay_acitvity: minimumOrderAmount <= 0}">
                            <span class="gotopay_button_style" v-if="minimumOrderAmount > 0">还差¥{{minimumOrderAmount}}起送</span>
                            <router-link :to="{path:'/confirmOrder', query:{geohash, shopId}}" class="gotopay_button_style" v-else >去结算</router-link>
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
                            <section class="cart_food_details" id="cartFood">
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
                                            <span @click="removeOutCart(item.category_id, item.item_id, item.food_id, item.name, item.price, item.specs)">
                                                <svg>
                                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                                                </svg>
                                            </span>
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
                </section>
            </transition>
            <transition name="fade-choose">
                <section class="rating_container" id="ratingContainer" v-show="changeShowType =='rating'">
                    <section v-load-more="loaderMoreRating" type="2">
                        <section>

                            <header class="rating_header">
                                <section class="rating_header_left">
                                    <p>{{shopDetailData.rating}}</p>
                                    <p>综合评价</p>
                                    <p>高于周边商家{{(ratingScoresData.compare_rating*100).toFixed(1)}}%</p>
                                </section>
                                <section class="rating_header_right">
                                    <p>
                                        <span>服务态度</span>
                                        <rating-star :rating='ratingScoresData.service_score'></rating-star>
                                        <span class="rating_num">{{ratingScoresData.service_score.toFixed(1)}}</span>
                                    </p>
                                    <p>
                                        <span>菜品评价</span>
                                        <rating-star :rating='ratingScoresData.food_score'></rating-star>
                                        <span class="rating_num">{{ratingScoresData.food_score.toFixed(1)}}</span>
                                    </p>
                                    <p>
                                        <span>送达时间</span>
                                        <span class="delivery_time">{{shopDetailData.order_lead_time}}分钟</span>
                                    </p>
                                </section>
                            </header>
                            <ul class="tag_list_ul">
                                <li v-for="(item, index) in ratingTagsList" :key="index" :class="{unsatisfied: item.unsatisfied, tagActivity: ratingTageIndex == index}" @click="changeTgeIndex(index, item.name)">{{item.name}}({{item.count}})</li>
                            </ul>
                            <ul class="rating_list_ul">
                                <li v-for="(item, index) in ratingList" :key="index" class="rating_list_li">
                                    <img :src="getImgPath(item.avatar)" class="user_avatar">
                                    <section class="rating_list_details">
                                        <header>
                                            <section class="username_star">
                                                <p class="username">{{item.username}}</p>
                                                <p class="star_desc">
                                                    <rating-star :rating='item.rating_star'></rating-star>
                                                    <span class="time_spent_desc">{{item.time_spent_desc}}</span>
                                                </p>
                                            </section>
                                            <time class="rated_at">{{item.rated_at}}</time>
                                        </header>
                                        <ul class="food_img_ul">
                                            <li v-for="(item, index) in item.item_ratings" :key="index">
                                                <img :src="getImgPath(item.image_hash)" v-if="item.image_hash">
                                            </li>
                                        </ul>
                                        <ul class="food_name_ul">
                                            <li v-for="(item, index) in item.item_ratings" :key="index" class="ellipsis">
                                                {{item.food_name}}
                                            </li>
                                        </ul>
                                    </section>
                                </li>
                            </ul>
                        </section>
                    </section>
                </section>
            </transition>
        </section>
        <section>
            <transition name="fade">
                <div class="specs_cover" @click="showChooseList" v-if="showSpecs"></div>
            </transition>
            <transition name="fadeBounce">
                <div class="specs_list" v-if="showSpecs">
                    <header class="specs_list_header">
                        <h4 class="ellipsis">{{choosedFoods.name}}</h4>
                        <svg width="16" height="16" xmlns="http://www.w3.org/2000/svg" version="1.1"class="specs_cancel" @click="showChooseList">
                            <line x1="0" y1="0" x2="16" y2="16"  stroke="#666" stroke-width="1.2"/>
                            <line x1="0" y1="16" x2="16" y2="0"  stroke="#666" stroke-width="1.2"/>
                        </svg>
                    </header>
                    <section class="specs_details">
                        <h5 class="specs_details_title">{{choosedFoods.specifications[0].name}}</h5>
                        <ul>
                            <li v-for="(item, itemIndex) in choosedFoods.specifications[0].values" :class="{specs_activity: itemIndex == specsIndex}" @click="chooseSpecs(itemIndex)">
                                {{item}}
                            </li>
                        </ul>
                    </section>
                    <footer class="specs_footer">
                        <div class="specs_price">
                            <span>¥ </span>
                            <span>{{choosedFoods.specfoods[specsIndex].price}}</span>
                        </div>
                        <div class="specs_addto_cart" @click="addSpecs(choosedFoods.category_id, choosedFoods.item_id, choosedFoods.specfoods[specsIndex].food_id, choosedFoods.specfoods[specsIndex].name, choosedFoods.specfoods[specsIndex].price, choosedFoods.specifications[0].values[specsIndex], choosedFoods.specfoods[specsIndex].packing_fee, choosedFoods.specfoods[specsIndex].sku_id, choosedFoods.specfoods[specsIndex].stock)">加入购物车</div>
                    </footer>
                </div>
            </transition>
        </section>
        <transition name="fade">
            <p class="show_delete_tip" v-if="showDeleteTip">多规格商品只能去购物车删除哦</p>
        </transition>
        <transition
        appear
        @after-appear = 'afterEnter'
        @before-appear="beforeEnter"
        v-for="(item,index) in showMoveDot"
        >
            <span class="move_dot" v-if="item">
                <svg class="move_liner">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-add"></use>
                </svg>
            </span>
        </transition>
       <loading v-show="showLoading || loadRatings"></loading>
       <section class="animation_opactiy shop_back_svg_container" v-if="showLoading">
           <img src="../../images/shop_back_svg.svg">
       </section>
       <transition name="router-slid" mode="out-in">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import {msiteAddress, shopDetails, foodMenu, getRatingList, ratingScores, ratingTags} from 'src/service/getData'
    import loading from 'src/components/common/loading'
    import buyCart from 'src/components/common/buyCart'
    import ratingStar from 'src/components/common/ratingStar'
    import {loadMore, getImgPath} from 'src/components/common/mixin'
    import {imgBaseUrl} from 'src/config/env'
    import BScroll from 'better-scroll'

    export default {
        data(){
            return{
                geohash: '', //geohash位置信息
                shopId: null, //商店id值
                showLoading: true, //显示加载动画
                changeShowType: 'food',//切换显示商品或者评价
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
                ratingList: null, //评价列表
                ratingOffset: 0, //评价获取数据offset值
                ratingScoresData: null, //评价总体分数
                ratingTagsList: null, //评价分类列表
                ratingTageIndex: 0, //评价分类索引
                preventRepeatRequest: false,// 防止多次触发数据请求
                ratingTagName: '',//评论的类型
                loadRatings: false, //加载更多评论是显示加载组件
                foodScroll: null,  //食品列表scroll
                showSpecs: false,//控制显示食品规格
                specsIndex: 0, //当前选中的规格索引值
                choosedFoods: null, //当前选中视频数据
                showDeleteTip: false, //多规格商品点击减按钮，弹出提示框
                showMoveDot: [], //控制下落的小圆点显示隐藏
                windowHeight: null, //屏幕的高度
                elLeft: 0, //当前点击加按钮在网页中的绝对top值
                elBottom: 0, //当前点击加按钮在网页中的绝对left值
                ratingScroll: null, //评论页Scroll
                imgBaseUrl,
            }
        },
        created(){
            this.geohash = this.$route.query.geohash;
            this.shopId = this.$route.query.id;
            this.INIT_BUYCART();
        },
        mounted(){
            this.initData();
            this.windowHeight = window.innerHeight;
        },
        beforeDestroy(){
            // this.foodScroll.removeEventListener('scroll', )
        },
        mixins: [loadMore, getImgPath],
        components: {
            loading,
            ratingStar,
            buyCart,
        },
        computed: {
            ...mapState([
                'latitude','longitude','cartList'
            ]),
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
            //当前商店购物信息
            shopCart: function (){
                return {...this.cartList[this.shopId]};
            },
            //购物车中总共商品的数量
            totalNum: function (){
                let num = 0;
                this.cartFoodList.forEach(item => {
                    num += item.num
                })
                return num
            },
        },
        methods: {
            ...mapMutations([
                'RECORD_ADDRESS','ADD_CART','REDUCE_CART','INIT_BUYCART','CLEAR_CART','RECORD_SHOPDETAIL'
            ]),
            //初始化时获取基本数据
            async initData(){
                if (!this.latitude) {
                    //获取位置信息
                    let res = await msiteAddress(this.geohash);
                    // 记录当前经度纬度进入vuex
                    this.RECORD_ADDRESS(res);
                }
                //获取商铺信息
                this.shopDetailData = await shopDetails(this.shopId, this.latitude, this.longitude);
                //获取商铺食品列表
                this.menuList = await foodMenu(this.shopId);
                //评论列表
                this.ratingList = await getRatingList(this.shopId, this.ratingOffset);
                //商铺评论详情
                this.ratingScoresData = await ratingScores(this.shopId);
                //评论Tag列表
                this.ratingTagsList = await ratingTags(this.shopId);
                this.RECORD_SHOPDETAIL(this.shopDetailData)
                //隐藏加载动画
                this.hideLoading();
            },
            //获取食品列表的高度，存入shopListTop
            getFoodListHeight(){
                const listContainer = this.$refs.menuFoodList;
                if (listContainer) {
                  const listArr = Array.from(listContainer.children[0].children);
                  listArr.forEach((item, index) => {
                      this.shopListTop[index] = item.offsetTop;
                  });
                  this.listenScroll(listContainer)
                }
            },
            //当滑动食品列表时，监听其scrollTop值来设置对应的食品列表标题的样式
            listenScroll(element){
                this.foodScroll = new BScroll(element, {
                    probeType: 3,
                    deceleration: 0.001,
                    bounce: false,
                    swipeTime: 2000,
                    click: true,
                });

                const wrapperMenu = new BScroll('#wrapper_menu', {
                    click: true,
                });

                const wrapMenuHeight = this.$refs.wrapperMenu.clientHeight;
                this.foodScroll.on('scroll', (pos) => {
                    if (!this.$refs.wrapperMenu) {
                        return
                    }
                    this.shopListTop.forEach((item, index) => {
                        if (this.menuIndexChange && Math.abs(Math.round(pos.y)) >= item) {
                            this.menuIndex = index;
                            const menuList=this.$refs.wrapperMenu.querySelectorAll('.activity_menu');
                            const el = menuList[0];
                            wrapperMenu.scrollToElement(el, 800, 0, -(wrapMenuHeight/2 - 50));
                        }
                    })
                })
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
                this.foodScroll.scrollTo(0, -this.shopListTop[index], 400);
                this.foodScroll.on('scrollEnd', () => {
                    this.menuIndexChange = true;
                })
            },
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
                let newArr = [];
                let cartFoodNum = 0;
                this.totalPrice = 0;
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
                this.totalPrice = this.totalPrice.toFixed(2);
                this.categoryNum = [...newArr];
            },
            //控制购物列表是否显示
            toggleCartList(){
                this.cartFoodList.length ? this.showCartList = !this.showCartList : true;
            },
            //清除购物车
            clearCart(){
                this.toggleCartList();
                this.CLEAR_CART(this.shopId);
            },
            //监听圆点是否进入购物车
            listenInCart(){
                if (!this.receiveInCart) {
                    this.receiveInCart = true;
                    this.$refs.cartContainer.addEventListener('animationend', () => {
                        this.receiveInCart = false;
                    })
                    this.$refs.cartContainer.addEventListener('webkitAnimationEnd', () => {
                        this.receiveInCart = false;
                    })
                }
            },
            //获取不同类型的评论列表
            async changeTgeIndex(index, name){
                this.ratingTageIndex = index;
                this.ratingOffset = 0;
                this.ratingTagName = name;
                let res = await getRatingList(this.shopId, this.ratingOffset, name);
                this.ratingList = [...res];
                this.$nextTick(() => {
                    this.ratingScroll.refresh();
                })
            },
            //加载更多评论
            async loaderMoreRating(){
                if (this.preventRepeatRequest) {
                    return
                }
                this.loadRatings = true;
                this.preventRepeatRequest = true;
                this.ratingOffset += 10;
                let ratingDate = await getRatingList(this.shopId, this.ratingOffset, this.ratingTagName);
                this.ratingList = [...this.ratingList,...ratingDate];
                this.loadRatings = false;
                if (ratingDate.length >= 10) {
                    this.preventRepeatRequest = false;
                }
            },
            //隐藏动画
            hideLoading(){
                this.showLoading = false;
            },
            //显示规格列表
            showChooseList(foods){
                if (foods) {
                    this.choosedFoods = foods;
                }
                this.showSpecs = !this.showSpecs;
                this.specsIndex = 0;
            },
            //记录当前所选规格的索引值
            chooseSpecs(index){
                this.specsIndex = index;
            },
            //多规格商品加入购物车
            addSpecs(category_id, item_id, food_id, name, price, specs, packing_fee, sku_id, stock){
                this.ADD_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs, packing_fee, sku_id, stock});
                this.showChooseList();
            },
            //显示提示，无法减去商品
            showReduceTip(){
                this.showDeleteTip = true;
                clearTimeout(this.timer);
                this.timer = setTimeout(() => {
                    clearTimeout(this.timer);
                    this.showDeleteTip = false;
                }, 3000);
            },
            //显示下落圆球
            showMoveDotFun(showMoveDot, elLeft, elBottom){
                this.showMoveDot = [...this.showMoveDot, ...showMoveDot];
                this.elLeft = elLeft;
                this.elBottom = elBottom;
            },
            beforeEnter(el){
                el.style.transform = `translate3d(0,${37 + this.elBottom - this.windowHeight}px,0)`;
                el.children[0].style.transform = `translate3d(${this.elLeft - 30}px,0,0)`;
                el.children[0].style.opacity = 0;
            },
            afterEnter(el){
                el.style.transform = `translate3d(0,0,0)`;
                el.children[0].style.transform = `translate3d(0,0,0)`;
                el.style.transition = 'transform .55s cubic-bezier(0.3, -0.25, 0.7, -0.15)';
                el.children[0].style.transition = 'transform .55s linear';
                this.showMoveDot = this.showMoveDot.map(item => false);
                el.children[0].style.opacity = 1;
                el.children[0].addEventListener('transitionend', () => {
                    this.listenInCart();
                })
                el.children[0].addEventListener('webkitAnimationEnd', () => {
                    this.listenInCart();
                })
            },
            goback(){
                this.$router.go(-1);
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
            shopCart: function (value){
                this.initCategoryNum();
            },
            //购物车列表发生变化，没有商铺时，隐藏
            cartFoodList: function (value){
                if(!value.length){
                    this.showCartList = false;
                }
            },
            //商品、评论切换状态
            changeShowType: function (value){
                if (value === 'rating') {
                    this.$nextTick(() => {
                        this.ratingScroll = new BScroll('#ratingContainer', {
                            probeType: 3,
                            deceleration: 0.003,
                            bounce: false,
                            swipeTime: 2000,
                            click: true,
                        });
                        this.ratingScroll.on('scroll', (pos) => {
                            if (Math.abs(Math.round(pos.y)) >=  Math.abs(Math.round(this.ratingScroll.maxScrollY))) {
                                this.loaderMoreRating();
                                this.ratingScroll.refresh();
                            }
                        })
                    })
                }
            }
        }
    }
</script>

<style lang="scss" scoped>
    @import 'src/style/mixin';
    @keyframes mymove{
       0%   { transform: scale(1) }
       25%  { transform: scale(.8) }
       50%  { transform: scale(1.1) }
       75%  { transform: scale(.9) }
       100% { transform: scale(1) }
    }
    @-moz-keyframes mymove{
       0%   { transform: scale(1) }
       25%  { transform: scale(.8) }
       50%  { transform: scale(1.1) }
       75%  { transform: scale(.9) }
       100% { transform: scale(1) }
    }
    @-webkit-keyframes mymove{
       0%   { transform: scale(1) }
       25%  { transform: scale(.8) }
       50%  { transform: scale(1.1) }
       75%  { transform: scale(.9) }
       100% { transform: scale(1) }
    }
    @-o-keyframes mymove{
       0%   { transform: scale(1) }
       25%  { transform: scale(.8) }
       50%  { transform: scale(1.1) }
       75%  { transform: scale(.9) }
       100% { transform: scale(1) }
    }
    .shop_back_svg_container{
        position: fixed;
        @include wh(100%, 100%);
        img{
            @include wh(100%, 100%);
        }
    }
    .shop_container{
        display: flex;
        flex-direction: column;
        position: absolute;
        right: 0;
        left: 0;
        height: 100%;
    }
    .goback{
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 2rem;
        z-index: 11;
        padding-top: 0.2rem;
        padding-left: 0.2rem;
    }
    .shop_detail_header{
        // overflow: hidden;
        position: relative;
        .header_cover_img_con {
          height: 100%;
          overflow: hidden;
          position: absolute;
          width: 100%;
          .header_cover_img{
              width: 100%;
              position: absolute;
              top: 0;
              left: 0;
              z-index: 9;
              filter: blur(10px);
          }
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
                        width: 11.5rem;
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
                padding-right: 1rem;
                p{
                    @include sc(.5rem, #fff);
                    span{
                        color: #fff;
                    }
                    .tip_icon{
                        padding: 0 .04rem;
                        border: 0.025rem solid #fff;
                        border-radius: 0.1rem;
                        font-size: .4rem;
                        display: inline-block;
                    }
                }
                .ellipsis{
                    width: 84%;
                }
                .footer_arrow{
                    @include wh(.45rem, .45rem);
                    position: absolute;
                    right: .3rem;
                }
            }
        }
    }
    .activities_details{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #262626;
        z-index: 200;
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
                    padding: 0 .02rem;
                    display: inline-block;
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

    .food_container{
        display: flex;
        flex: 1;
        padding-bottom: 2rem;
        overflow: hidden;
    }
    .menu_container{
        display: flex;
        flex: 1;
        overflow-y: hidden;
        position: relative;
        .menu_left{
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
                                    p{
                                        white-space: nowrap;
                                    }
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
                                        transform: scale(0.8) translate(0.1rem, -.1rem);
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
                display: flex;
                background-color: #3d3d3f;
                position: absolute;
                padding: .4rem;
                border: 0.18rem solid #444;
                border-radius: 50%;
                left: .5rem;
                top: -.7rem;
                .cart_icon{
                    @include wh(1.2rem, 1.2rem);
                }
                .cart_list_length{
                    position: absolute;
                    top: -.25rem;
                    right: -.25rem;
                    background-color: #ff461d;
                    line-height: .7rem;
                    text-align: center;
                    border-radius: 50%;
                    border: 0.025rem solid #ff461d;
                    min-width: .7rem;
                    height: .7rem;
                    @include sc(.5rem, #fff);
                    font-family: Helvetica Neue,Tahoma,Arial;
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
            max-height: 20rem;
            overflow-y: auto;
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
                    span{
                        display: flex;
                        align-items: center;
                        justify-content: center;
                    }
                    svg{
                        @include wh(.9rem, .9rem);
                        fill: #3190e8;
                    }
                    .specs_reduce_icon{
                        fill: #999;
                    }
                    .cart_num{
                        @include sc(.65rem, #666);
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
    .change_show_type{
        display: flex;
        background-color: #fff;
        padding: .3rem 0 .6rem;
        border-bottom: 1px solid #ebebeb;
        div{
            flex: 1;
            text-align: center;
            span{
                @include sc(.65rem, #666);
                padding: .2rem .1rem;
                border-bottom: 0.12rem solid #fff;
            }
            .activity_show{
                color: #3190e8;
                border-color: #3190e8;
            }
        }
    }
    .rating_container{
        flex: 1;
        overflow-y: hidden;
        flex-direction: column;
        p, span, li, time{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
        .rating_header{
            display: flex;
            background-color: #fff;
            padding: .8rem .5rem;
            margin-bottom: 0.5rem;
            .rating_header_left{
                flex: 3;
                text-align: center;
                p:nth-of-type(1){
                    @include sc(1.2rem, #f60);
                }
                p:nth-of-type(2){
                    @include sc(.65rem, #666);
                    margin-bottom: .1rem;
                }
                p:nth-of-type(3){
                    @include sc(.4rem, #999);
                }
            }
            .rating_header_right{
                flex: 4;
                p{
                    font-size: .65rem;
                    line-height: 1rem;
                    display: flex;
                    align-items: center;
                    justify-content: flex-start;
                    span:nth-of-type(1){
                        color: #666;
                        margin-right: .5rem;
                    }
                    .rating_num{
                        width: 3rem;
                        @include sc(.55rem, #f60);
                    }
                    .delivery_time{
                        @include sc(.4rem, #999);
                    }
                }
            }
        }
        .tag_list_ul{
            display: flex;
            flex-wrap: wrap;
            background-color: #fff;
            padding: .5rem;
            li{
                @include sc(.6rem, #6d7885);
                padding: .3rem .3rem;
                background-color: #ebf5ff;
                border-radius: 0.2rem;
                border: 1px;
                margin: 0 .4rem .2rem 0;
            }
            .unsatisfied{
                background-color: #f5f5f5;
                color: #aaa;
            }
            .tagActivity{
                background-color: #3190e8;
                color: #fff;
            }
        }
        .rating_list_ul{
            background-color: #fff;
            padding: 0 .5rem;
            .rating_list_li{
                border-top: 1px solid #f1f1f1;
                display: flex;
                padding: .6rem 0;
                .user_avatar{
                    @include wh(1.5rem, 1.5rem);
                    border: 0.025rem;
                    border-radius: 50%;
                    margin-right: .8rem;
                }
                .rating_list_details{
                    flex: 1;
                    header{
                        display: flex;
                        flex: 1;
                        justify-content: space-between;
                        margin-bottom: .3rem;
                        .username_star{
                            @include sc(.55rem, #666);
                            .username{
                                color: #666;
                                margin-bottom: .2rem;
                            }
                            .star_desc{
                                display: flex;
                                align-items: center;
                                .time_spent_desc{
                                    @include sc(.55rem, #666)
                                    margin-left: .15rem;
                                }
                            }
                        }
                        .rated_at{
                            @include sc(.4rem, #999);
                        }
                    }
                    .food_img_ul{
                        display: flex;
                        flex-wrap: wrap;
                        margin-bottom: .4rem;
                        li{
                            img{
                                @include wh(3rem, 3rem);
                                margin-right: .4rem;
                                display: block;
                            }
                        }
                    }
                    .food_name_ul{
                        display: flex;
                        flex-wrap: wrap;
                        li{
                            @include sc(.55rem, #999);
                            width: 2.2rem;
                            padding: .2rem;
                            border: 0.025rem solid #ebebeb;
                            border-radius: 0.15rem;
                            margin-right: .3rem;
                            margin-bottom: 4px;
                        }
                    }
                }
            }
        }
    }
    .specs_cover{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: rgba(0,0,0,.4);
        z-index: 17;
    }
    .specs_list{
        position: fixed;
        top: 35%;
        left: 15%;
        width: 70%;
        background-color: #fff;
        z-index: 18;
        border: 1px;
        border-radius: 0.2rem;
        .specs_list_header{
            h4{
                @include sc(.7rem, #222);
                font-weight: normal;
                text-align: center;
                padding: .5rem;
            }
            .specs_cancel{
                position: absolute;
                right: .5rem;
                top: .5rem;
            }
        }
        .specs_details{
            padding: .5rem;
            .specs_details_title{
                @include sc(.6rem, #666);
            }
            ul{
                display: flex;
                flex-wrap: wrap;
                padding: .4rem 0;
                li{
                    font-size: .6rem;
                    padding: .3rem .5rem;
                    border: 0.025rem solid #ddd;
                    border-radius: .2rem;
                    margin-right: .5rem;
                    margin-bottom: .2rem;
                }
                .specs_activity{
                    border-color: #3199e8;
                    color: #3199e8;
                }
            }
        }
        .specs_footer{
            @include fj;
            align-items: center;
            background-color: #f9f9f9;
            padding: 0.5rem;
            border: 1px;
            border-bottom-left-radius: .2rem;
            border-bottom-right-radius: .2rem;
            .specs_price{
                span{
                    color: #ff6000;
                }
                span:nth-of-type(1){
                    font-size: .5rem;
                }
                span:nth-of-type(2){
                    font-size: .8rem;
                    font-weight: bold;
                    font-family: Helvetica Neue,Tahoma;
                }
            }
            .specs_addto_cart{
                @include wh(4rem, 1.3rem);
                background-color: #3199e8;
                border: 1px;
                border-radius: 0.15rem;
                @include sc(.6rem, #fff);
                text-align: center;
                line-height: 1.3rem;
            }
        }
    }
    .show_delete_tip{
        position: fixed;
        top: 50%;
        left: 15%;
        width: 70%;
        transform: translateY(-50%);
        background-color: rgba(0,0,0,.8);
        z-index: 18;
        @include sc(.65rem, #fff);
        text-align: center;
        padding: .5rem 0;
        border: 1px;
        border-radius: 0.25rem;
    }
    .move_dot{
        position: fixed;
        bottom: 30px;
        left: 30px;

        svg{
            @include wh(.9rem, .9rem);
            fill: #3190e8;
        }
    }
    .fade-enter-active, .fade-leave-active {
        transition: opacity .5s;
    }
    .fade-enter, .fade-leave-active {
        opacity: 0;
    }
    .fade-choose-enter-active, .fade-choose-leave-active {
        transition: opacity .5s;
    }
    .fade-choose-leave, .fade-choose-leave-active {
        display: none;
    }
    .fade-choose-enter, .fade-choose-leave-active {
        opacity: 0;
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translate3d(2rem, 0, 0);
        opacity: 0;
    }
    .toggle-cart-enter-active, .toggle-cart-leave-active {
        transition: all .3s ease-out;
    }
    .toggle-cart-enter, .toggle-cart-leave-active {
        transform: translateY(100%);
    }

</style>
