 <template>
    <section class="cart_module">
        <section v-if="!foods.specifications.length" class="cart_button">
            <transition name="showReduce">
                <svg @click="removeOutCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id, foods.specfoods[0].name, foods.specfoods[0].price, '')" v-if="foodNum">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                </svg>
            </transition>
            <transition name="fade">
                <span class="cart_num" v-if="foodNum">{{foodNum}}</span>
            </transition>
            <svg @click="addToCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id, foods.specfoods[0].name, foods.specfoods[0].price, '', $event)">
                <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-add"></use>
            </svg>
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
        </section>
        <section v-else class="choose_specification">
            <section class="choose_icon_container">
                <transition name="showReduce">
                    <svg class="specs_reduce_icon" v-if="foodNum" @click="showReduceTip">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                    </svg>
                </transition>
                <transition name="fade">
                    <span class="cart_num" v-if="foodNum">{{foodNum}}</span>
                </transition>
                <transition name="fade">
                    <p class="show_delete_tip" v-if="showDeleteTip">多规格商品只能去购物车删除哦</p>
                </transition>
                <span class="show_chooselist" @click="showChooseList">选规格</span>
            </section>
            <section>
                <transition name="fade">
                    <div class="specs_cover" @click="showChooseList" v-if="showSpecs"></div>
                </transition>
                <transition name="fadeBounce">
                    <div class="specs_list" v-if="showSpecs">
                        <header class="specs_list_header">
                            <h4 class="ellipsis">{{foods.name}}</h4>
                            <svg width="16" height="16" xmlns="http://www.w3.org/2000/svg" version="1.1"class="specs_cancel" @click="showChooseList">
                                <line x1="0" y1="0" x2="16" y2="16"  stroke="#666" stroke-width="1.2"/>
                                <line x1="0" y1="16" x2="16" y2="0"  stroke="#666" stroke-width="1.2"/>
                            </svg>
                        </header>
                        <section class="specs_details">
                            <h5 class="specs_details_title">{{foods.specifications[0].name}}</h5>
                            <ul>
                                <li v-for="(item, itemIndex) in foods.specifications[0].values" :class="{specs_activity: itemIndex == specsIndex}" @click="chooseSpecs(itemIndex)">
                                    {{item}}
                                </li>
                            </ul>
                        </section>
                        <footer class="specs_footer">
                            <div class="specs_price">
                                <span>¥ </span>
                                <span>{{foods.specfoods[specsIndex].price}}</span>
                            </div>
                            <div class="specs_addto_cart" @click="addSpecs(foods.category_id, foods.item_id, foods.specfoods[specsIndex].food_id, foods.specfoods[specsIndex].name, foods.specfoods[specsIndex].price, foods.specifications[0].values[specsIndex])">加入购物车</div>
                        </footer>
                    </div>
                </transition>
            </section>
        </section>
    </section>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    export default {
    	data(){
            return{
               showSpecs: false,//控制显示食品规格
               specsIndex: 0, //当前选中的规格索引值
               showDeleteTip: false, //多规格商品点击减按钮，弹出提示框
               showMoveDot: [], //控制下落的小圆点显示隐藏
               elLeft: 0, //当前点击加按钮在网页中的绝对top值
               elBottom: 0, //当前点击加按钮在网页中的绝对left值
               windowHeight: null, //屏幕的高度
            }
        },
        mounted(){
            this.windowHeight = window.innerHeight;
        },
        computed: {
            ...mapState([
                'cartList'
            ]),
            /**
             * 监听cartList变化，更新当前商铺的购物车信息shopCart，同时返回一个新的对象
             */
            shopCart: function (){
                return Object.assign({},this.cartList[this.shopId]);
            },
            //shopCart变化的时候重新计算当前商品的数量
            foodNum: function (){
                let category_id = this.foods.category_id;
                let item_id = this.foods.item_id;
                if (this.shopCart&&this.shopCart[category_id]&&this.shopCart[category_id][item_id]) {
                    let num = 0;
                    Object.values(this.shopCart[category_id][item_id]).forEach((item,index) => {
                        num += item.num;
                    })
                    return num;
                }else {
                    return 0;
                }
            },
        },
        props:['foods', 'shopId'],
        methods: {
            ...mapMutations([
                'ADD_CART','REDUCE_CART',
            ]),
            //移出购物车
            removeOutCart(category_id, item_id, food_id, name, price, specs){
                if (this.foodNum > 0) {
                    this.REDUCE_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs});
                }
            },
            //加入购物车，计算按钮位置。
            addToCart(category_id, item_id, food_id, name, price, specs, event){
                this.ADD_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs});
                this.elLeft = event.target.getBoundingClientRect().left;
                this.elBottom = event.target.getBoundingClientRect().bottom;
                this.showMoveDot.push(true);

            },
            //显示规格列表
            showChooseList(){
                this.showSpecs = !this.showSpecs;
                this.specsIndex = 0;
            },
            //记录当前所选规格的索引值
            chooseSpecs(index){
                this.specsIndex = index;
            },
            //多规格商品加入购物车
            addSpecs(category_id, item_id, food_id, name, price, specs){
                this.ADD_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs});
                this.showChooseList();
            },
            //点击多规格商品的减按钮，弹出提示
            showReduceTip(){
                this.showDeleteTip = true;
                clearTimeout(this.timer);
                this.timer = setTimeout(() => {
                    clearTimeout(this.timer);
                    this.showDeleteTip = false;
                }, 3000);
            },
            beforeEnter(el){
                el.style.transform = `translate3d(0,${39 + this.elBottom - this.windowHeight}px,0)`;
                el.children[0].style.transform = `translate3d(${this.elLeft - 40}px,0,0)`;
            },
            afterEnter(el){
                el.style.transform = `translate3d(0,0,0)`;
                el.children[0].style.transform = `translate3d(0,0,0)`;
                el.style.transition = 'all .55s cubic-bezier(0.3, -0.19, 0.65, -0.15)';
                el.children[0].style.transition = 'all .55s linear';
                //圆点到达目标点后移出
                this.showMoveDot = this.showMoveDot.map(item => false);
                //监听运动结束，通知父级进行后续操作
                el.children[0].addEventListener('transitionend', () => {
                    this.$emit('moveInCart')
                })
            }
        },
    }
</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';
	.cart_module{
        .cart_button{
            display: flex;
            align-items: center;
            .move_dot{
                position: fixed;
                bottom: 40px;
                left: 40px;
                z-index: 11;
            }
        }
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
        .choose_specification{
            .choose_icon_container{
                display: flex;
                align-items: center;
                .show_chooselist{
                    display: block;
                    @include sc(.55rem, #fff);
                    padding: .2rem .3rem;
                    background-color: $blue;
                    border-radius: 0.5rem;
                    border: 1px solid $blue;
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
        }
    }
    .showReduce-enter-active, .showReduce-leave-active {
        transition: all .3s ease-out;
    }
    .showReduce-enter, .showReduce-leave-active {
        opacity: 0;
        transform: translateX(1rem);
    }
    .fade-enter-active, .fade-leave-active {
        transition: all .3s;
    }
    .fade-enter, .fade-leave-active {
        opacity: 0;
    }
    .fadeBounce-enter-active, .fadeBounce-leave-active {
        transition: all .3s;
    }
    .fadeBounce-enter, .fadeBounce-leave-active {
        opacity: 0;
        transform: scale(.7);
    }
    
</style>

