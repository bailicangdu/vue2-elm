 <template>
    <section class="cart_module">
        <section v-if="!foods.specifications.length" class="cart_button">
            <transition name="showReduce">
                <svg @click="removeOutCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id, foods.specfoods[0].name, foods.specfoods[0].price, '', foods.specfoods[0].packing_fee, foods.specfoods[0].sku_id, foods.specfoods[0].stock)" v-if="foodNum">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                </svg>
            </transition>
            <transition name="fade">
                <span class="cart_num" v-if="foodNum">{{foodNum}}</span>
            </transition>
            <svg @click="addToCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id, foods.specfoods[0].name, foods.specfoods[0].price, '', foods.specfoods[0].packing_fee, foods.specfoods[0].sku_id, foods.specfoods[0].stock, $event)">
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
                <span class="show_chooselist" @click="showChooseList(foods)">选规格</span>
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
            removeOutCart(category_id, item_id, food_id, name, price, specs, packing_fee, sku_id, stock){
                if (this.foodNum > 0) {
                    this.REDUCE_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs, packing_fee, sku_id, stock});
                }
            },
            //加入购物车，计算按钮位置。
            addToCart(category_id, item_id, food_id, name, price, specs, packing_fee, sku_id, stock, event){
                this.ADD_CART({shopid: this.shopId, category_id, item_id, food_id, name, price, specs, packing_fee, sku_id, stock});
                this.elLeft = event.target.getBoundingClientRect().left;
                this.elBottom = event.target.getBoundingClientRect().bottom;
                this.showMoveDot.push(true);

            },
            //显示规格列表
            showChooseList(foodScroll){
                this.showSpecs = !this.showSpecs;
                this.$emit('showChooseList', foodScroll)
            },
            //点击多规格商品的减按钮，弹出提示
            showReduceTip(){
                this.$emit('showReduceTip')
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
    @import '../../style/mixin';
	.cart_module{
        .cart_button{
            display: flex;
            align-items: center;
            .move_dot{
                position: fixed;
                bottom: 40px;
                left: 40px;
                z-index: 99999999999999999;
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

