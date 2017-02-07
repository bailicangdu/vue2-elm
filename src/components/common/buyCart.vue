 <template>
    <section class="cart_module">
        <section v-if="!foods.specifications.length" class="cart_button">
            <transition name="showReduce">
                <svg class="cart_minus" @click="removeOutCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id)" v-if="foodNum">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-minus"></use>
                </svg>
            </transition>
            <transition name="showNum">
                <span class="cart_num" v-if="foodNum">{{foodNum}}</span>
            </transition>
            <svg class="cart_add" @click="addToCart(foods.category_id, foods.item_id, foods.specfoods[0].food_id)">
                <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#cart-add"></use>
            </svg>
        </section>
        <section v-else class="choose_specification">
            选规格
        </section>
    </section>
</template>

<script>
    export default {
    	data(){
            return{
               foodNum: 0,
            }
        },
        created(){
            this.foodNum = this.getIintNum(this.foods.category_id, this.foods.item_id);
        },
        props:['foods', 'shopCart', 'index'],
        methods: {
            removeOutCart(category_id, item_id, food_id){
                if (this.foodNum > 0) {
                    this.foodNum --;
                    this.$emit('reduce', category_id, item_id, food_id, this.index);
                }
            },
            addToCart(category_id, item_id, food_id){
                this.foodNum ++ ;
                this.$emit('add', category_id, item_id, food_id, this.index);
            },
            getIintNum(category_id, item_id){
                if (this.shopCart&&this.shopCart[category_id]&&this.shopCart[category_id][item_id]) {
                    let num = 0;
                    Object.values(this.shopCart[category_id][item_id]).forEach((item,index) => {
                        num += item;
                    })
                    return num;
                }else {
                    return 0;
                }
            }
        }
    }
</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';
	.cart_module{
        .cart_button{
            display: flex;
            align-items: center;
            .showReduce-enter-active, .showReduce-leave-active {
                transition: all .5s ease-out;
            }
            .showReduce-enter, .showReduce-leave-active {
                opacity: 0;
                transform: translateX(1rem);
            }
            .showNum-enter-active, .showNum-leave-active {
                transition: all .5s;
            }
            .showNum-enter, .showNum-leave-active {
                opacity: 0;
            }
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
</style>
