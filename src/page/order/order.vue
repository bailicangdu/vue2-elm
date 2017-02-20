 <template>
    <div class="order_page">
        <head-top head-title="订单列表" go-back='true'></head-top>
        <ul class="order_list_ul" v-load-more="loaderMore">
            <li class="order_list_li" v-for="item in orderList" :key="item.id">
                <img :src="item.restaurant_image_url" class="restaurant_image">
                <section class="order_item_right">
                    <section @click="showDetail(item)">
                        <header class="order_item_right_header">
                            <section class="order_header">
                                <h4 >
                                    <span class="ellipsis">{{item.restaurant_name}} </span>
                                    <svg fill="#333" class="arrow_right">
                                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                                    </svg>
                                </h4>
                                <p class="order_time">{{item.formatted_created_at}}</p>
                            </section>
                            <p class="order_status">
                                {{item.status_bar.title}}
                            </p>
                        </header>
                        <section class="order_basket">
                            <p class="order_name ellipsis">{{item.basket.group[0][0].name}}{{item.basket.group[0].length > 1 ? ' 等' + item.basket.group[0].length + '件商品' : ''}}</p>
                            <p class="order_amount">¥{{item.total_amount.toFixed(2)}}</p>
                        </section>
                    </section>
                    <router-link :to="{path: '/shop', query: {geohash, id: item.restaurant_id}}" tag="div" class="order_again">
                        <span>再来一单</span>
                    </router-link>
                </section>
            </li>
        </ul>
        <foot-guide></foot-guide>
        <transition name="loading">
            <loading v-show="showLoading"></loading>
        </transition>
        <transition name="router-slid">
            <router-view></router-view>
        </transition>   
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import headTop from '../../components/header/head'
    import loading from '../../components/common/loading'
    import {getImgPath} from '../../components/common/mixin'
    import footGuide from '../../components/footer/footGuide'
    import {getOrderList} from '../../service/getData'
    import {loadMore} from '../../components/common/mixin'

    export default {
      data(){
            return{
                orderList: null,
                offset: 0,
                preventRepeat: false, 
                showLoading: true, //显示加载动画
            }
        },
        mounted(){
            this.initData();

        },
        mixins: [loadMore],
        components: {
            headTop,
            footGuide,
            loading,
        },
        computed: {
            ...mapState([
                'userInfo', 'geohash'
            ]),
        },
        methods: {
             ...mapMutations([
               'SAVE_ORDER'
            ]),
            async initData(){
                this.orderList = await getOrderList(111, this.offset);
                this.showLoading = false;
            },
            async loaderMore(){
                if (this.preventRepeat) {
                    return
                }
                this.preventRepeat = true;
                this.showLoading = true;
                this.offset += 10;
                let res = await getOrderList(111, this.offset);
                this.orderList = this.orderList.concat(this.orderList, res);
                this.preventRepeat = false;
                this.showLoading = false;
            },
            showDetail(item){
                this.SAVE_ORDER(item);
                this.$router.push('/order/orderDetail');
            },
        }
    }
</script>
  
<style lang="scss" scoped>
    @import '../../style/mixin';
    
    .order_page{
        background-color: #f1f1f1;
        padding-bottom: 1.95rem;
        p, span, h4{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .order_list_ul{
       
        padding-top: 1.95rem;
        .order_list_li{
            background-color: #fff;
            display: flex;
            margin-bottom: 0.5rem;
            padding: .6rem .6rem 0;
            .restaurant_image{
                @include wh(2rem, 2rem);
                margin-right: 0.4rem;
            }
            .order_item_right{
                flex: 5;
                .order_item_right_header{
                    border-bottom: 0.025rem solid #f5f5f5;
                    padding-bottom: .3rem;
                    @include fj;
                    .order_header{
                        h4{
                            display: flex;
                            align-items: center;
                            justify-content: flex-start;
                            @include sc(.75rem, #333);
                            line-height: 1rem;
                            width: 9rem;

                            .arrow_right{
                                @include wh(.4rem, .4rem);
                                fill: #ccc;
                                margin-right: .2rem;
                            }
                        }
                        .order_time{
                            @include sc(.55rem, #999);
                            line-height: .8rem;
                        }
                    }
                    .order_status{
                        @include sc(.6rem, #333);
                    }
                }
                .order_basket{
                    @include fj;
                    line-height: 2rem;
                    border-bottom: 0.025rem solid #f5f5f5;
                    .order_name{
                        @include sc(.6rem, #666);
                        width: 10rem;
                    }
                    .order_amount{
                        @include sc(.6rem, #f60);
                        font-weight: bold;
                    }
                }
                .order_again{
                    text-align: right;
                    line-height: 1.6rem;
                    span{
                        @include sc(.55rem, #3190e8);
                        border: 0.025rem solid #3190e8;
                        padding: .1rem .2rem;
                        border-radius: .15rem;
                    }
                }
            }
        }
    }
    .loading-enter-active, .loading-leave-active {
        transition: opacity .7s
    }
    .loading-enter, .loading-leave-active {
        opacity: 0
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translateX(100%);
    }
</style>
