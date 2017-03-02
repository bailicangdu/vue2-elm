 <template>
  <div class="rating_page">
        <head-top head-title="我的优惠" go-back='true'></head-top>
        <section v-if="!showLoading">
            <section class="category_title"> 
                <span :class="{choosed: categoryType === 1}" @click="categoryType = 1">红包</span>
                <span :class="{choosed: categoryType === 2}" @click="categoryType = 2">商家代金券</span>
            </section>
            <transition name="router-fade" mode="out-in">
                <section v-if="categoryType === 1">
                    <section class="hongbao_container">
                        <header class="hongbao_title">
                            <section class="total_number">
                                有 <span>{{hongbaoList.length}}</span> 个红包即将到期
                            </section>
                            <section class="hongbao_description">
                                <img src="../../images/description.png" height="24" width="24">
                                <span class="hongbao_detail">红包说明</span>
                            </section>
                        </header>
                        <ul class="hongbao_list_ul">
                            <li class="hongbao_list_li" v-for="item in hongbaoList" :key="item.id">
                                <section class="list_item">
                                    <div class="list_item_left">
                                        <span>¥</span>
                                        <span>{{String(item.amount).split('.')[0]}}</span>
                                        <span>.</span>
                                        <span>{{String(item.amount).split('.')[1]||0}}</span>
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
                    <router-link to="/home" class="history_hongbao">
                        <span>查看历史红包</span>
                        <svg class="history_right">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </router-link>
                    <footer class="hongbao_footer">
                        <router-link to="/home" class="hongbao_style" style="border-right: 1px solid #f5f5f5">
                            兑换红包
                        </router-link>
                        <router-link to="/home" class="hongbao_style">
                            推荐有奖
                        </router-link>
                    </footer>
                </section>
            </transition>
            <transition name="router-fade" mode="out-in">
                <section v-if="categoryType === 2">
                    
                </section>
            </transition>
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
                .hongbao_detail{
                    color: $blue;
                }
            }
        }
        .hongbao_list_ul{
            .hongbao_list_li{
                background: #fff url(../../images/hongbao.png) repeat-x ;
                background-size: .5rem .2rem;
                margin-bottom: 0.5rem;
                border-radius: 0.25rem;
                .list_item{
                    @include fj;
                    padding: 1rem 0.5rem .8rem;
                    .list_item_left{
                        font-size: 0;
                        border-right: 0.025rem dotted #ccc;
                        flex: 1;
                        span:nth-of-type(1){
                            @include sc(.75rem, #ff5340);
                            font-weight: bold;
                        }
                        span:nth-of-type(2){
                            @include sc(2rem, #ff5340);
                        }
                        span:nth-of-type(3), span:nth-of-type(4){
                            @include sc(1rem, #ff5340);
                            font-weight: bold;
                        }
                    }
                    .list_item_right{
                        flex: 2;
                        margin-left: 1.5rem;
                        h4{
                            @include sc(.7rem, #666);
                            margin-left: -.7rem;
                        }
                        ul{
                            li{
                                list-style-type: disc;
                                @include sc(.4rem, #999);
                            }
                        }
                    }
                }
                .list_item_footer{
                    background-color: #f9f9f9;
                    padding: .4rem .4rem;
                    border-bottom-left-radius: 0.25rem;
                    border-bottom-right-radius: 0.25rem;
                    ul{
                        li{
                            list-style-type: disc;
                            @include sc(.4rem, #999);
                            margin-left: .4rem;
                        }
                    }
                }
            }
        }
    }
    .history_hongbao{
        padding: .5rem 0 2.8rem;
        display: flex;
        align-items: center;
        justify-content: center;
        span{
            @include sc(.5rem, #999);
            margin-right: .2rem;
        }
        .history_right{
            @include wh(.4rem, .4rem);
            fill: #aaa;
        }
    }
    .hongbao_footer{
        position: fixed;
        bottom: 0;
        width: 100%;
        background-color: #fff;
        display: flex;
        .hongbao_style{
            flex: 1;
            line-height: 2rem;
            text-align: center;
            @include sc(.7rem, #555);
        }
    }
    .router-fade-enter-active, .router-fade-leave-active {
        transition: opacity .3s;
    }
    .router-fade-enter, .router-fade-leave-active {
        opacity: 0;
    }
</style>
