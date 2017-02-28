 <template>
  <div class="page_container">
        <head-top head-title="我的优惠" go-back='true'></head-top>
        <section>我的优惠</section>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {getOrderList} from 'src/service/getData'

    export default {
      data(){
            return{
    
            }
        },
        mounted(){

        },
        computed: {
            ...mapState([
                'userInfo', 
            ]),
        },
        components: {
            headTop,
        },
        methods: {
            ...mapMutations([
                'SAVE_AVANDER'
            ]),
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .page_container{
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #fff;
        z-index: 202;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    
</style>
