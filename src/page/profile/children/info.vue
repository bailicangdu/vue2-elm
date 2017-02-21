 <template>
  <div class="rating_page">
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
            <router-link to="/forget" class="info-router">
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
            <section class="exitlogin" @click="exitlogin">退出登录</section>
        </section>

        <section class="coverpart" v-if="show">
            <section class="cover-background"></section>
            <section class="cover-content " :class="{'cover-animate' : isEnter, 'cover-animate-leave' : isLeave}">
                <div class="sa-icon">
                    <span class="sa-body"></span>
                    <span class="sa-dot"></span>
                </div>
                <h2>是否退出登录</h2>
                <div class="sa-botton">
                    <button class="waiting" @click="waitingThing">再等等</button>
                    <div style="display:inline-block;">
                        <button class="quitlogin"  @click="outLogin">退出登录</button>
                    </div>
                </div>
            </section>
        </section>
    </div>
</template>

<script>
    import {mapMutations} from 'vuex'
    import headTop from '../../../components/header/head'
    import {getImgPath} from '../../../components/common/mixin'

    export default {
      data(){
            return{
                infoname:'',    //用户名
                infotel:'',     //用户手机
                show:false,
                isEnter:true,
                isLeave:false,
            }
        },
        created(){

        },
        beforeDestroy(){
            clearTimeout(this.timer)
        },
        mixins: [getImgPath],
        components: {
            headTop, 

        },
        props:[],
        methods: {
            ...mapMutations([
                'OUT_LOGIN'
            ]),
            exitlogin(){
                this.show=true;
                this.isEnter=true;
                this.isLeave=false;
            },
            waitingThing(){
                clearTimeout(this.timer)
                this.isEnter=false;
                this.isLeave=true;
                this.timer = setTimeout(() =>{
                    clearTimeout(this.timer)
                    this.show=false;
                },200)
            },
            outLogin(){
                this.OUT_LOGIN();
                this.waitingThing();
                this.$router.go(-1);
            },
        }
    }
</script>
  
<style lang="scss" scoped>
    @import '../../../style/mixin.scss';
  
    .rating_page{
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
            @include borderRadius(5px);
            text-align:center;
            background:#d8584a;
            @include sc(.6rem,#fff);

        }
        .exitlogin:active{
            opacity:.8;
            background:#C1C1C1;
        }
    }
    .info-router{
        display:block;
    }
    .coverpart{
        @include wh(100%,100%);
        @include allcover;
        .cover-background{
            @include wh(100%,100%);
            @include allcover;
            background:#000;
            z-index:100;
            opacity:.2;
        }
        .cover-content{
            width:94%;
            background:$fc;
            padding:17px;
            position:absolute;
            top:20%;
            left:3%;
            z-index:1000;
            @include borderRadius(5px);
            .sa-icon{
                @include wh(90px,90px);
                border:4px solid #f8bb86;
                @include borderRadius(50%);
                margin:20px auto;
                position:relative;
                .sa-body{
                    @include wh(5px,47px);
                    @include borderRadius(2px);
                    background:#f8bb86;
                    @include cl;
                    top:10px;
                }
                .sa-dot{
                    @include wh(7px,7px);
                    @include borderRadius(50%);
                    background:#f8bb86;
                    @include cl;
                    bottom:10px;
                }
            }
            h2{
                width:100%;
                text-align:center;
                @include sc(30px,#575757);
                font-weight:500;
                margin:25px 0;
            }
            .sa-botton{
                width:100%;
                text-align:center;

                button{
                    display:inline-block;
                    padding:.4rem 1rem;
                    @include borderRadius(5px);
                    @include sc(.6rem,$fc);
                    letter-spacing:1px;
                    margin-top:26px;
                }
                .waiting{
                    background:#C1C1C1;
                    margin-right:.4rem;
                }
                .quitlogin{
                    background:rgb(221, 107, 85);
                }
            }

        }
    }
    @-webkit-keyframes bounceIn {
  from, 20%, 40%, 60%, 80%, 100% {
    -webkit-animation-timing-function: cubic-bezier(0.215, 0.610, 0.355, 1.000);
    animation-timing-function: cubic-bezier(0.215, 0.610, 0.355, 1.000);
  }

  0% {
    opacity: 0;
    -webkit-transform: scale3d(.3, .3, .3);
    transform: scale3d(.3, .3, .3);
  }

  20% {
    -webkit-transform: scale3d(1.1, 1.1, 1.1);
    transform: scale3d(1.1, 1.1, 1.1);
  }

  40% {
    -webkit-transform: scale3d(.9, .9, .9);
    transform: scale3d(.9, .9, .9);
  }

  60% {
    opacity: 1;
    -webkit-transform: scale3d(1.03, 1.03, 1.03);
    transform: scale3d(1.03, 1.03, 1.03);
  }

  80% {
    -webkit-transform: scale3d(.97, .97, .97);
    transform: scale3d(.97, .97, .97);
  }

  100% {
    opacity: 1;
    -webkit-transform: scale3d(1, 1, 1);
    transform: scale3d(1, 1, 1);
  }
}

@keyframes bounceIn {
  from, 20%, 40%, 60%, 80%, 100% {
    -webkit-animation-timing-function: cubic-bezier(0.215, 0.610, 0.355, 1.000);
    animation-timing-function: cubic-bezier(0.215, 0.610, 0.355, 1.000);
  }

  0% {
    opacity: 0;
    -webkit-transform: scale3d(.3, .3, .3);
    transform: scale3d(.3, .3, .3);
  }

  20% {
    -webkit-transform: scale3d(1.1, 1.1, 1.1);
    transform: scale3d(1.1, 1.1, 1.1);
  }

  40% {
    -webkit-transform: scale3d(.9, .9, .9);
    transform: scale3d(.9, .9, .9);
  }

  60% {
    opacity: 1;
    -webkit-transform: scale3d(1.03, 1.03, 1.03);
    transform: scale3d(1.03, 1.03, 1.03);
  }

  80% {
    -webkit-transform: scale3d(.97, .97, .97);
    transform: scale3d(.97, .97, .97);
  }

  100% {
    opacity: 1;
    -webkit-transform: scale3d(1, 1, 1);
    transform: scale3d(1, 1, 1);
  }
}
@-webkit-keyframes zoomOut {
  from {
    opacity: 1;
  }

  50% {
    opacity: 0;
    -webkit-transform: scale3d(.3, .3, .3);
    transform: scale3d(.3, .3, .3);
  }

  100% {
    opacity: 0;
  }
}

@keyframes zoomOut {
  from {
    opacity: 1;
  }

  50% {
    opacity: 0;
    -webkit-transform: scale3d(.3, .3, .3);
    transform: scale3d(.3, .3, .3);
  }

  100% {
    opacity: 0;
  }
}
body .coverpart .cover-animate{
    transition:all 1s;
    animation:bounceIn .6s;
}
body .coverpart .cover-animate-leave{
    animation:zoomOut .4s;
}
</style>
