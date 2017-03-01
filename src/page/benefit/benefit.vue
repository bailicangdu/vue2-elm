 <template>
  <div class="rating_page">
        <head-top head-title="我的优惠" go-back='true'></head-top>
        <section v-if="!showLoading">
            <section class="category_title"> 
                <span :class="{choosed: categoryType === 1}">红包</span>
                <span>商家代金券</span>
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
                hongbao: null,
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
                this.hongbao = await getHongbaoNum(this.userInfo.user_id);
                this.showLoading = false;
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
        }
        .choosed{
            border-bottom: 0.1rem solid $blue;
            color: $blue;
        }
    }
    
</style>
