 <template>
    <div class="shop_container">
        <header class="shop_detail_header" v-if="!showLoading" ref="shopheader">
            <img :src="getImgPath(shopDetailData.image_path)" class="header_cover_img">
            <section class="description_header">
                <section class="description_top" @click="showActivitiesFun">
                    <section class="description_left">
                        <img :src="getImgPath(shopDetailData.image_path)">
                    </section>
                    <section class="description_right">
                        <h4 class="description_title ellipsis">{{shopDetailData.name}}</h4>
                        <p class="description_text">商家配送／{{shopDetailData.order_lead_time}}分钟送达／配送费¥{{shopDetailData.float_delivery_fee}}</p>
                        <p class="description_promotion ellipsis">公告：{{shopDetailData.promotion_info}}</p>
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
                <transition name="fadeactivities">
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
                            <p>{{shopDetailData.promotion_info}}</p>
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
                        <transition name="fade_title">
                            <p class="description_tip" v-show="index == TitleDetailIndex">
                                <span>{{item.name}}</span>
                                {{item.description}}
                            </p>
                        </transition>
                    </header>
                    <section v-for="(foods,index) in item.foods" :key="index" class="menu_detail_list">
                        <router-link to="/shop/rating" tag="div" class="menu_detail_link">
                            <section class="menu_food_img">
                                <img :src="getImgPath(foods.image_path)">
                            </section>
                            <section class="menu_food_description">
                                <h3 class="food_description_head">
                                    <strong class="description_foodname">{{foods.name}}</strong>
                                    <ul v-if="foods.attributes.length" class="attributes_ul">
                                        <li v-for="(attribute, index) in foods.attributes" :key="index" :style="{color: '#' + attribute.icon_color,borderColor:'#' +attribute.icon_color}" :class="{attribute_new: attribute.icon_name == '新'}">
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
                            <section class="cart_module">
                                <section v-if="!foods.specifications.length" class="cart_button">
                                    <svg class="cart_minus">
                                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                                    </svg>
                                    <span class="cart_num">1</span>
                                    <svg class="cart_add">
                                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-add"></use>
                                    </svg>
                                </section>
                                <section v-else class="choose_specification">
                                    选规格
                                </section>
                            </section>
                        </footer>
                    </section>
                </li>
            </ul>
        </section>
        <section class="buy_cart_container">
            <section class="cart_icon_container">
                <svg class="cart_icon">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-icon"></use>
                </svg>
            </section>
            <section class="cart_num">
                <div>¥ 0</div>
                <div>配送费¥{{deliveryFee}}</div>
            </section>
            <section class="gotopay">
                <span>还差¥{{minimumOrderAmount}}起送</span>
            </section>
        </section>
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
            }
        },
        created(){
            this.geohash = this.$route.query.geohash;
            this.shopId = this.$route.query.id;
        },
        mounted(){
            this.initData();
        },
        mixins: [getImgPath],
        components: {
            loading,
            ratingStar,
        },
        computed: {
            ...mapState([
                'latitude','longitude','cartList'
            ]),
            deliveryFee: function () {
                if (this.shopDetailData) {
                    return this.shopDetailData.float_delivery_fee;
                }else{
                    return null;
                }
            },
            minimumOrderAmount: function () {
                if (this.shopDetailData) {
                    return this.shopDetailData.float_minimum_order_amount;
                }else{
                    return null;
                }
            },
        },
        methods: {
            ...mapMutations([
                'RECORD_ADDRESS'
            ]),
            //初始化时获取基本数据
            async initData(){
                if (!this.latitude) {
                    //获取位置信息
                    let res = await msiteAdress(this.geohash);
                    // 记录当前经度纬度进入vuex
                    this.RECORD_ADDRESS(res);
                }
                this.shopDetailData = await shopDetails(this.shopId, this.latitude, this.longitude);
                this.menuList = await foodMenu(this.shopId);
                this.showLoading = false;
            },
            getFoodListHeight(){
                const baseHeight = this.$refs.shopheader.clientHeight;
                const listContainer = this.$refs.menuFoodList;
                const listArr = Array.from(listContainer.children);
                listArr.forEach((item, index) => {
                    this.shopListTop[index] = item.offsetTop - baseHeight;
                });
                this.listenScroll(listContainer)
            },
            listenScroll(element){
                let oldScrollTop;
                let requestFram;
                element.addEventListener('scroll',() => {
                   currenIndex();
                }, false)
                //运动过程中保持坚挺 scrollTop 的值
                element.addEventListener('touchmove',() => {
                   currenIndex();
                })
                //运动结束时判断是否有惯性运动
                element.addEventListener('touchend',() => {
                    oldScrollTop = element.scrollTop;
                    bounceMove();
                })
                
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

                const currenIndex = () => {
                    this.shopListTop.forEach((item, index) => {
                        if (this.menuIndexChange && element.scrollTop  >= item && element.scrollTop < this.shopListTop[index + 1]) {
                            this.menuIndex = index;
                        }
                    })
                }
            },
            showActivitiesFun(){
                this.showActivities = !this.showActivities;
            },
            chooseMenu(index){
                this.menuIndex = index;
                this.menuIndexChange = false;
                animate(this.$refs.menuFoodList, {scrollTop: this.shopListTop[index]}, () => {
                    this.menuIndexChange = true;
                });
            },
            showTitleDetail(index){
                if (this.TitleDetailIndex == index) {
                    this.TitleDetailIndex = null;
                }else{
                    this.TitleDetailIndex = index;
                }
            }
        },
        watch: {
            showLoading: function (value){
                if (!value) {
                    this.$nextTick(() => {
                        this.getFoodListHeight();
                    })
                }
            }
        }
    }
</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    .buy_cart_container{
        position: absolute;
        background-color: #3d3d3f;
        bottom: 0;
        left: 0;
        @include wh(100%, 2rem);
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
        .gotopay{
            position: absolute;
            right: 0;
            background-color: #535356;
            @include wh(5rem, 100%);
            text-align: center;
            display: flex;
            align-items: center;
            justify-content: center;
            span{
                @include sc(.65rem, #fff);
                font-weight: bold;
            }
        }
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
                z-index: 12;
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
            
            .fadeactivities-enter-active, .fadeactivities-leave-active {
                transition: opacity .5s;
            }
            .fadeactivities-enter, .fadeactivities-leave-active {
                opacity: 0;
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
                img{
                    @include wh(.5rem, .6rem);
                }
                span{
                    @include sc(.6rem, #666);
                }
            }
            .activity_menu{
                border-left: 0.15rem solid #3190e8;
                background-color: #fff;
                span{
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
                .fade_title-enter-active, .fade_title-leave-active {
                    transition: opacity .5s
                }
                .fade_title-enter, .fade_title-leave-active {
                    opacity: 0
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
                    .cart_module{
                        .cart_button{
                            display: flex;
                            align-items: center;
                            svg{
                                @include wh(.8rem, .8rem);
                                fill: #3190e8;
                            }
                            .cart_num{
                                @include sc(.6rem, #666);
                                min-width: 1rem;
                                text-align: center;
                            }
                        }
                        .choose_specification{
                            @include sc(.55rem, #fff);
                            padding: .2rem .3rem;
                            background-color: $blue;
                            border-radius: 0.5rem;
                            border: 1px solid $blue;
                        }
                    }
                }
            }
        }
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translateX(100%);
    }
</style>
