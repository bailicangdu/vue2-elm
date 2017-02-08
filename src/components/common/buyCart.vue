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
            <svg class="cart_add" @click="addToCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id, foods.specfoods[0].name, foods.specfoods[0].price, '')">
                <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-add"></use>
            </svg>
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
    export default {
    	data(){
            return{
               showSpecs: false,
               specsIndex: 0,
               showDeleteTip: false,
            }
        },
        created(){
        
        },
        computed: {
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
            }
        },
        props:['foods', 'shopCart'],
        methods: {
            removeOutCart(category_id, item_id, food_id, name, price, specs){
                if (this.foodNum > 0) {
                    this.$emit('reduce', category_id, item_id, food_id, name, price, specs);
                }
            },
            addToCart(category_id, item_id, food_id, name, price, specs){
                this.$emit('add', category_id, item_id, food_id, name, price, specs);
            },
            chooseSpecs(index){
                this.specsIndex = index;
            },
            showChooseList(){
                this.showSpecs = !this.showSpecs;
                this.specsIndex = 0;
            },
            addSpecs(category_id, item_id, food_id, name, price, specs){
                this.$emit('add', category_id, item_id, food_id, name, price, specs);
                this.showChooseList();
            },
            showReduceTip(){
                this.showDeleteTip = true;
                clearTimeout(this.timer);
                this.timer = setTimeout(() => {
                    clearTimeout(this.timer);
                    this.showDeleteTip = false;
                }, 3000)
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
