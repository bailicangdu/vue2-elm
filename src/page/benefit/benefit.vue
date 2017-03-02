 <template>
  <div class="rating_page">
        <head-top head-title="我的优惠" go-back='true'></head-top>
        <section v-if="!showLoading">
            <section class="category_title"> 
                <span :class="{choosed: categoryType === 1}" @click="categoryType = 1">红包</span>
                <span :class="{choosed: categoryType === 2}" @click="categoryType = 2">商家代金券</span>
            </section>
            <section class="hongbao_container">
                <header class="hongbao_title">
                    <section class="total_number">
                        有 <span>{{hongbaoList.length}}</span> 个红包即将到期
                    </section>
                    <section class="hongbao_description">
                        <img src="../../images/description.png" height="24" width="24">
                        <span>红包说明</span>
                    </section>
                </header>
                <ul class="hongbao_list_ul">
                    <li class="hongbao_list_li" v-for="item in hongbaoList" :key="item.id">
                        <section class="list_item">
                            <div class="list_item_left">
                                <span>¥</span><span>{{item.amount}}</span>
                            </div>
                            <div class="list_item_right">
                                <h4>{{item.name}}</h4>
                                <ul>
                                    <li v-for="(descriptions, index) in item.descriptions" :key="index">{{descriptions}}</li>
                                </ul>
                            </div>
                        </section>
                        <footer class="list_item_footer" v-if="item.extra_limit">
                            <ul>
                                <li v-for="(limit, index) in item.extra_limit" :key="index">
                                    {{limit}}
                                </li>
                            </ul>
                        </footer>
                    </li>
                </ul>
            </section>
        </section>
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
        <loading v-show="showLoading"></loading>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {getHongbaoNum} from 'src/service/getData'
    import alertTip from 'src/components/common/alertTip'
    import loading from 'src/components/common/loading'

    export default {
      data(){
            return{
                showAlert: false,
                alertText: null,
                showLoading: true,
                hongbaoList: null,
                categoryType: 1,

            }
        },
        mounted(){
            this.initData();
        },
        components: {
            headTop,
            alertTip,
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
                if (this.userInfo) {
                    this.hongbaoList = await getHongbaoNum(this.userInfo.user_id);
                    this.showLoading = false;
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
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .category_title{
        display: flex;
        justify-content: space-around;
        align-items: center;
        height: 2rem;
        background-color: #fff;
        span{
            text-align: center;
            @include sc(.65rem, #333);
            padding-bottom: .2rem;
            border-bottom: 0.1rem solid #fff;
        }
        .choosed{
            border-bottom-color: $blue;
            color: $blue;
        }
    }
    .hongbao_container{
        padding: 0 .5rem;
        .hongbao_title{
            @include fj;
            font-size: .5rem;
            line-height: 2rem;
            .total_number{
                color: #666;
                span{
                    color: #ff5340;
                }
            }
            .hongbao_description{
                display: flex;
                align-items: center;
                img{
                    @include wh(.6rem, .6rem);
                    margin-right: .2rem;
                }
                span{
                    color: $blue;
                }
            }
        }
        .hongbao_list_ul{
            .hongbao_list_li{
                
            }
        }
    }
</style>
