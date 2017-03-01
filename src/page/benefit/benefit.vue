 <template>
  <div class="rating_page">
        <head-top head-title="我的优惠" go-back='true'></head-top>
        <section>我的余额</section>
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
        <loading v-show="showLoading"></loading>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {payRequest} from 'src/service/getData'
    import alertTip from 'src/components/common/alertTip'
    import loading from 'src/components/common/loading'

    export default {
      data(){
            return{
                showAlert: false,
                alertText: null,
                showLoading: true,
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
    
</style>
