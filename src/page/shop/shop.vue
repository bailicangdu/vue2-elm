 <template>
    <div>
        <header class="shop_detail_header" v-if="!showLoading">
            <img :src="getImgPath(shopDetailData.image_path)" class="header_cover_img">
            <section class="description_header">
                <section class="description_top">
                    <section class="description_left">
                        <img :src="getImgPath(shopDetailData.image_path)">
                    </section>
                    <section class="description_right">
                        <h4 class="description_title ellipsis">{{shopDetailData.name}}</h4>
                        <p class="description_text">商家配送／{{shopDetailData.order_lead_time}}分钟送达／配送费¥{{shopDetailData.float_delivery_fee}}</p>
                        <p class="description_promotion ellipsis">公告：{{shopDetailData.promotion_info}}</p>
                    </section>
                </section>
                <footer class="description_footer" v-if="shopDetailData.activities.length">
                    <p class="ellipsis">
                        <span class="tip_icon" :style="{backgroundColor: '#' + shopDetailData.activities[0].icon_color, borderColor: '#' + shopDetailData.activities[0].icon_color}">{{shopDetailData.activities[0].icon_name}}</span>
                        <span>{{shopDetailData.activities[0].description}}（APP专享）</span>
                    </p>
                    <p>{{shopDetailData.activities.length}}个活动</p>
                </footer>
            </section>
            <svg width="14" height="14" xmlns="http://www.w3.org/2000/svg" version="1.1" class="description_arrow" >
                <path d="M0 0 L8 7 L0 14"  stroke="#fff" stroke-width="1" fill="none"/>
            </svg>
        </header>
       <loading v-if="showLoading"></loading>
    </div>
</template>

<script>
    import {mapState, mapMutations} from 'vuex'
    import {imgBaseUrl} from '../../config/env'
    import {msiteAdress, shopDetails} from '../../service/getData'
    import loading from '../../components/common/loading'
    import {getImgPath} from '../../components/common/mixin'
    
    export default {
        data(){
            return{
                geohash: '', //geohash位置信息
                shopId: null, //商店id值
                showLoading: true, //显示加载动画
                shopDetailData: null, //商铺详情
            }
        },
        created(){
            this.geohash = this.$route.query.geohash;
            this.shopId = this.$route.query.id;
        },
        async mounted(){
            if (!this.latitude) {
                //获取位置信息
                let res = await msiteAdress(this.geohash);
                // 记录当前经度纬度进入vuex
                this.RECORD_ADDRESS(res);
            }
            this.shopDetailData = await shopDetails(this.shopId, this.latitude, this.longitude);
            this.showLoading = false;
        },
        mixins: [getImgPath],
        components: {
            loading
        },
        computed: {
            ...mapState([
                'latitude','longitude'
            ])
        },
        methods: {
            ...mapMutations([
                'RECORD_ADDRESS'
            ]),
        }

    }
</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';
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
            padding: 0.43rem;
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
            }
            .description_footer{
                @include fj;
                margin-top: 0.5rem;
                p{
                    @include sc(.5rem, #fff);
                    span{
                        color: #fff;
                    }
                    .tip_icon{
                        padding: .01rem;
                        border: 0.025rem solid #fff;
                        border-radius: 0.1rem;
                    }
                }
                .ellipsis{
                    width: 87%;
                }
            }
        }
        .description_arrow{
            @include ct;
            right: 0.3rem;
            z-index: 11;

        }
    }
</style>
