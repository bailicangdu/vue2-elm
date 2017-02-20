 <template>
  <div class="info_page">
        <head-top head-title="账户信息" go-back='true'></head-top>
        <section class="profile-info">
            <section class="headportrait">
                <input type="file" class="profileinfopanel-upload">
                <h2>头像</h2>
                <div class="headportrait-div">
                    <span class="headportrait-div-top">
                        <svg class="">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#avatar-default"></use>
                        </svg>
                    </span>
                    <span class="headportrait-div-bottom">
                        <svg fill="#d8d8d8">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </div>
            </section>
            <router-link to="" class="info-router">
                <section class="headportrait headportraitwo">
                        <h2>用户名</h2>
                        <div class="headportrait-div">
                            <p>fhsdfhdj</p>
                            <span class="headportrait-div-bottom">
                                <svg fill="#d8d8d8">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                                </svg>
                            </span>
                        </div>
                </section>
            </router-link>
            <router-link to="" class="info-router">
                <section class="headportrait headportraitwo headportraithree">
                        <h2>收货地址</h2>
                        <div class="headportrait-div">
                            <span class="headportrait-div-bottom">
                                <svg fill="#d8d8d8">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                                </svg>
                            </span>
                        </div>
                </section>
            </router-link>
            <section class="bind-phone">
                账号绑定
            </section>
            <router-link to="" class="info-router">
                <section class="headportrait headportraitwo headportraithree">
                        <h2><img src="../../../images/bindphone.png" style="display:inline-block;margin-right:.4rem;" alt="">手机</h2>
                        <div class="headportrait-div">
                            <p>15800319949</p>
                            <span class="headportrait-div-bottom">
                                <svg fill="#d8d8d8">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                                </svg>
                            </span>
                        </div>
                </section>
            </router-link>
            <section class="bind-phone">
                安全设置
            </section>
            <router-link to="" class="info-router">
                <section class="headportrait headportraitwo headportraithree">
                        <h2>登录密码</h2>
                        <div class="headportrait-div">
                            <p>修改</p>
                            <span class="headportrait-div-bottom">
                                <svg fill="#d8d8d8">
                                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                                </svg>
                            </span>
                        </div>
                </section>
            </router-link>
            <section class="exitlogin">退出登录</section>
        </section>
        

    </div>
</template>

<script>
    import headTop from '../../../components/header/head'
    import {getImgPath} from '../../../components/common/mixin'

    export default {
      data(){
            return{
    
            }
        },
        created(){

        },
        mixins: [getImgPath],
        components: {
            headTop,

        },
        props:[],
        methods: {
            
        }
    }
</script>
  
<style lang="scss" scoped>
    @import '../../../style/mixin.scss';
  
    .info_page{
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #f2f2f2;
        z-index: 202;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .profile-info{
        @include wh(100%,3.1rem);
        .profileinfopanel-upload{
            display: block;
            position: absolute;
            opacity: 0;
            top: 2.35rem;
            left: 0;
            @include wh(100%,3.1rem);
        }
        .headportrait{
            margin-top:.4rem;
            padding:.5rem .4rem;
            @include fj(space-between);
            align-items:center;
            border-top:1px solid #ddd;
            background:#fff;

            h2{
                @include sc(.6rem,#333);
                font-weight:500;
                display:flex;
                align-items:center;
            }
            .headportrait-div{
                span{
                    display:inline-block;
                    
                    svg{
                        @include wh(100%,100%);
                    }
                }
                .headportrait-div-top{
                    @include wh(2rem,2rem);
                    vertical-align:middle;
                }
                .headportrait-div-bottom{
                    @include wh(.66667rem,1.4rem);
                    position:relative;
                    top:0.44rem;
                }
            }
        }
        .headportraitwo{
            margin-top:0;
            padding:.3rem .4rem;
            .headportrait-div{
                @include fj(left) 
                p{
                    text-align:left;
                    line-height:1.39rem;
                    @include sc(.7rem,#999);
                    margin-right:.2rem;
                    font-weight:100;
                    font-family:Arial;
                }
                .headportrait-div-bottom{
                    top:0;
                }
            }
        }
        .headportraithree{
            border-bottom:1px solid #ddd;
        }
        .bind-phone{
            padding:.4rem .4rem;
            @include sc(.5rem,#666);
        }
        .exitlogin{
            width:96%;
            margin:1.3rem auto 0;
            line-height:1.5rem;
            border-radius:5px;
            text-align:center;
            background:#d8584a;
            @include sc(.6rem,#fff);

        }
        .exitlogin:active{
            opacity:.8;
        }
    }
    .info-router{
        display:block;
    }
</style>
