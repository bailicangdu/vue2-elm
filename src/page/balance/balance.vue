 <template>
  <div class="page">
        <head-top head-title="我的余额" go-back='true'></head-top>
        <section class="content_container">
            <section class="content">
                <header class="content_header">
                    <span class="content_title_style">当前余额</span>
                    <section class="contetn_description">
                        <img src="../../images/description.png" height="24" width="24">
                        <router-link to="/balance/detail" class="content_title_style">余额说明</router-link>
                    </section>
                </header>
                <p class="content_num">
                    <span>0.00</span>
                    <span>元</span>
                </p>
                <div class="promit_button">提现</div>
            </section>
        </section>
        <p class="deal_detail">交易明细</p>
        <div class="no_log">
            <img src="../../images/no-log.png">
            <p>暂无明细记录</p>
        </div>
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
        <transition name="router-slid" mode="out-in">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import alertTip from 'src/components/common/alertTip'
    
    export default {
      data(){
            return{
                showAlert: false,
                alertText: null,
            }
        },
        mounted(){
          
        },
        components: {
            headTop,
            alertTip,
        },
        computed: {
           
        },
        methods: {
            
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .page{
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .content_container{
        padding: .3rem;
        background-color: $blue;
        .content{
            padding: .4rem;
            background-color: #fff;
            border-radius: .15rem;
            .content_header{
                @include fj;
                font-size: .55rem;
                .contetn_description{
                    display: flex;
                    align-items: center;
                    img{
                        @include wh(.6rem, .6rem);
                        margin-right: .2rem;
                    }
                    .content_title_style{
                        color: $blue;
                    }
                }
                .content_title_style{
                    color: #666;
                }
            }
            .content_num{
                span:nth-of-type(1){
                    @include sc(1.8rem, #333);
                }
                span:nth-of-type(2){
                    @include sc(.7rem, #333);
                }
            }
            .promit_button{
                @include wh(100%, 2rem);
                @include sc(.8rem, #fff);
                border-radius: 0.15rem;
                line-height: 2rem;
                margin-top: 1rem;
                text-align: center;
                background-color: #ccc;
            }
        }
    }
    .deal_detail{
        @include sc(.6rem, #999);
        line-height: 2rem;
        padding-left: .5rem;
    }
    .no_log{
        text-align: center;
        margin-top: 1rem;
        img{
            @include wh(8rem, 5rem);
        }
        p{
            margin-top: .5rem;
            @include sc(.7rem, #666);
        }
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translate3d(2rem, 0, 0);
        opacity: 0;
    }
</style>
