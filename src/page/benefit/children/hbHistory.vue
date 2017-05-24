 <template>
  <div class="rating_page">
        <head-top head-title="历史红包" go-back='true'></head-top>
        <section v-if="!showLoading" id="scroll_section" class="scroll_container">
            <ul class="hongbao_list_ul">
                <li class="hongbao_list_li" v-for="item in expiredList" :key="item.id">
                    <section class="list_item">
                        <div class="list_item_left">
                            <span>¥</span>
                            <span>{{String(item.amount).split('.')[0]}}</span>
                            <span>.</span>
                            <span>{{String(item.amount).split('.')[1]||0}}</span>
                            <p>{{item.description_map.sum_condition}}</p>
                        </div>
                        <div class="list_item_right">
                            <h4>{{item.name}}</h4>
                            <p>{{item.description_map.validity_periods}}</p>
                            <p>{{item.description_map.phone}}</p>
                        </div>
                    </section>
                    <footer class="list_item_footer" v-if="item.limit_map">
                        <p>{{item.limit_map.restaurant_flavor_ids}}</p>
                    </footer>
                    <svg class="expired">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#expired"></use>
                    </svg>
                </li>
            </ul>
        </section>
        <loading v-if="showLoading"></loading>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {getExpired} from 'src/service/getData'
    import loading from 'src/components/common/loading'
    import BScroll from 'better-scroll'
    
    export default {
      data(){
            return{
                showAlert: false,
                alertText: null,
                showLoading: true,
                expiredList: null, //历史红包列表
            }
        },
        mounted(){
            this.initData();
        },
        components: {
            headTop,
            loading,
        },
        computed: {
            ...mapState([
                'userInfo',
            ]),
        },
        methods: {
            ...mapMutations([
                'CLEAR_CART'
            ]),
            async initData(){
                //获取历史红包
                if (this.userInfo) {
                    this.expiredList = await getExpired(this.userInfo.user_id);
                    this.showLoading = false;
                    this.$nextTick(() => {
                        new BScroll('#scroll_section', {  
                            deceleration: 0.001,
                            bounce: true,
                            swipeTime: 1800,
                            click: true,
                        }); 
                    })
                }
            }
        },
        watch: {
            userInfo: function (value){
                this.initData();
            }
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .rating_page{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        padding-top: 1.95rem;
        z-index: 203;
        background-color: #f1f1f1;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .scroll_container{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        padding-top: 1.95rem;
    }
    .hongbao_list_ul{
        padding: 1rem .5rem;
        .hongbao_list_li{
            background: #fff url(../../../images/expired.png) repeat-x ;
            background-size: .5rem .2rem;
            margin-bottom: 0.5rem;
            border-radius: 0.25rem;
            position: relative;
            .list_item{
                @include fj;
                padding: 1rem 0.5rem .8rem;
                .list_item_left{
                    font-size: 0;
                    border-right: 0.025rem dotted #ccc;
                    flex: 1;
                    span:nth-of-type(1){
                        @include sc(.75rem, #ccc);
                        font-weight: bold;
                    }
                    span:nth-of-type(2){
                        @include sc(2rem, #ccc);
                    }
                    span:nth-of-type(3), span:nth-of-type(4){
                        @include sc(1rem, #ccc);
                        font-weight: bold;
                    }
                    p{
                        @include sc(0.4rem, #999);
                    }
                }
                .list_item_right{
                    flex: 2;
                    margin-left: 1.5rem;
                    h4{
                        @include sc(.7rem, #666);
                        margin-left: -.7rem;
                    }
                    p{
                        list-style-type: disc;
                        margin-left: -.7rem;
                        @include sc(.4rem, #999);   
                    }
                }
            }
            .list_item_footer{
                background-color: #f9f9f9;
                padding: .4rem .4rem;
                border-bottom-left-radius: 0.25rem;
                border-bottom-right-radius: 0.25rem;
                p{
                    list-style-type: disc;
                    @include sc(.4rem, #999);
                    margin-left: .4rem;
                }
            }
            .expired{
                fill: #ddd;
                @include wh(3rem, 3rem);
                top: .6rem;
                right: .2rem;
                position: absolute;
            }
        }
    }
</style>
