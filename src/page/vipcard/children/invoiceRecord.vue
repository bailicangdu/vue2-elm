 <template>
  <div class="page_container">
        <head-top head-title="购买记录" go-back='true'></head-top>
        <!-- 没有办过会员卡，获取不到数据，只好写死 -->
        <section class="invoice_contianer">
            <img src="../../../images/no-log.png">
            <p>没有购买记录</p>    
        </section>
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
        position: fixed;
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
    .invoice_contianer{
        display: flex;
        flex-direction: column;
        align-items: center;
        img{
            @include wh(8rem, 5rem);
            margin-top: 5rem;
        }
        p{
            @include sc(.6rem, #999);
            margin-top: .8rem;
        }
    }
</style>
