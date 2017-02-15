<template>
    <div>
        <head-top go-back='true' :head-title="profiletitle"></head-top>
        <section class="profile-number">
            <router-link to="/profile/info" class="profile-link">
                <span class="privateImage">
                    <svg class="privateImage-svg">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#avatar-default"></use>
                    </svg>
                </span>
                <div class="user-info">
                    <p>3adc5bfcd</p>
                    <p>
                        <span class="user-icon">
                            <svg class="icon-mobile" fill="#fff">
                                <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#mobile"></use>
                            </svg>
                        </span>
                        <span class="icon-mobile-number">15800319949</span>
                    </p>
                </div>
                <span class="arrow">
                    <svg class="arrow-svg" fill="#fff">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                    </svg>
                </span>
            </router-link>
        </section>
        <section class="info-data">
            <ul class="clear">
                <router-link to="/profile/balance" tag="li" class="info-data-link">
                    <span class="info-data-top"><b>0.00</b>元</span>
                    <span class="info-data-bottom">我的余额</span>
                </router-link>
                <router-link to="/profile/benefit" tag="li" class="info-data-link">
                    <span class="info-data-top"><b>1</b>个</span>
                    <span class="info-data-bottom">我的优惠</span>
                </router-link>
                <router-link to="/profile/points" tag="li" class="info-data-link">
                    <span class="info-data-top"><b>1010</b>分</span>
                    <span class="info-data-bottom">我的积分</span>
                </router-link>
            </ul>
        </section>
        <section class="profile-1reTe">
            <!-- 我的订单 -->
            <router-link to='/order' class="myorder">
                <aside>
                    <svg fill="#4aa5f0">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#order"></use>
                    </svg>
                </aside>
                <div class="myorder-div">
                    <span>我的订单</span>
                    <span class="myorder-divsvg">
                        <svg fill="#bbb">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </div>
            </router-link>
            <!-- 积分商城 -->
            <router-link to='/order' class="myorder">
                <aside>
                    <svg fill="#fc7b53">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#point"></use>
                    </svg>
                </aside>
                <div class="myorder-div">
                    <span>积分商城</span>
                    <span class="myorder-divsvg">
                        <svg fill="#bbb">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </div>
            </router-link>
            <!-- 饿了么会员卡 -->
            <router-link to='/vipcard' class="myorder">
                <aside>
                    <svg fill="#ffc636">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#vip"></use>
                    </svg>
                </aside>
                <div class="myorder-div">
                    <span>饿了么会员卡</span>
                    <span class="myorder-divsvg">
                        <svg fill="#bbb">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </div>
            </router-link>
        </section>
        <section class="profile-1reTe">
            <!-- 服务中心 -->
            <router-link to='/profile/service' class="myorder">
                <aside>
                    <svg fill="#4aa5f0">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#service"></use>
                    </svg>
                </aside>
                <div class="myorder-div">
                    <span>服务中心</span>
                    <span class="myorder-divsvg">
                        <svg fill="#bbb">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </div>
            </router-link>
            <!-- 下载饿了么APP -->
            <router-link to='/profile/service' class="myorder">
                <aside>
                    <svg fill="#3cabff">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#download"></use>
                    </svg>
                </aside>
                <div class="myorder-div" style="border-bottom:0;">
                    <span>下载饿了么APP</span>
                    <span class="myorder-divsvg">
                        <svg fill="#bbb">
                            <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right"></use>
                        </svg>
                    </span>
                </div>
            </router-link>
        </section>
        <transition name="router-slid">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
import headTop from '../../components/header/head'
import {mapState} from 'vuex'

export default {
    data(){
        return{
            profiletitle: '我的',
            count : '',       //优惠券个数
            pontNumber : '',  //积分
        }
    },

    mounted(){
        console.log(this.userInfo)
    },

    components:{
        headTop
    },

    computed:{
         ...mapState([
            'userInfo'
        ]),
    },

    methods:{

    },
}

</script>

<style lang="scss" scoped>
   @import '../../style/mixin'; 

   
   .profile-number{
        padding-top:1.95rem;
        .profile-link{
            display:block;
            display:flex;
            box-align: center;
            -webkit-box-align: center;
            -ms-flex-align: center;
            align-items: center;
            background:$blue;
            padding: .666667rem .6rem;
            .privateImage{
                display:inline-block;
                @include wh(2.5rem,2.5rem);
                border-radius:50%;
                vertical-align:middle;
                .privateImage-svg{
                    background:$fc;
                    border-radius:50%;
                    @include wh(2.5rem,2.5rem);
                }
            }
            .user-info{
                margin-left:.48rem;
                -webkit-box-flex: 1;
                -ms-flex-positive: 1;
                flex-grow: 1;
                p{
                    font-weight:700;
                    @include sc(.8rem,$fc);
                    .user-icon{
                        @include wh(0.5rem,0.75rem);
                        display:inline-block;
                        vertical-align:middle;
                        line-height:0.75rem;
                        .icon-mobile{
                            @include wh(100%,100%);
                        }
                    }
                    .icon-mobile-number{
                        display:inline-block;
                        @include sc(.57333rem,$fc);

                    }
                }
                
            }
            .arrow{
                @include wh(.46667rem,.98rem);
                display:inline-block;
                svg{
                   @include wh(100%,100%); 
                }
            }
        }
   }
   .info-data{
        width:100%;
        background:$fc;
        box-sizing: border-box;
        ul{
            .info-data-link{
                float:left;
                width:33.33%;
                display:inline-block;
                border-right:1px solid #f1f1f1;
                span{
                    display:block;
                    width:100%;
                    text-align:center;
                }
                .info-data-top{
                    @include sc(.55rem,#333);
                    padding: .853333rem 0 .453333rem;
                    b{
                        display:inline-block;
                        @include sc(1.2rem,#f90);
                        font-weight:700;
                        line-height:1rem;
                        font-family: Helvetica Neue,Tahoma;
                    }
                }
                .info-data-bottom{
                    @include sc(.57333rem,#666);
                    font-weight:400;
                    padding-bottom:.453333rem;

                }
            }
            .info-data-link:nth-of-type(2){
                .info-data-top{
                    b{
                        color:#ff5f3e;
                    }
                }
               
            }
            .info-data-link:nth-of-type(3){
                border:0;
                .info-data-top{
                    b{
                        color:#6ac20b;
                    }
                }
            }
        }   
   }
   .profile-1reTe{
        margin-top:.4rem;
        background:$fc;
        .myorder{
            padding-left:1.6rem;
            display:flex;
            align-items: center;
            aside{
                @include wh(.7rem,.7rem);
                margin-left:-.866667rem;
                margin-right:.266667rem;
                display:flex;
                align-items: center;
                svg{
                    @include wh(100%,100%);
                }
            }
            .myorder-div{
                width:100%;
                border-bottom:1px solid #f1f1f1;
                padding:.433333rem .266667rem .433333rem 0;
                @include sc(.7rem,#333);
                display:flex;
                justify-content:space-between;
                span{
                    display:block;
                }
                .myorder-divsvg{
                    @include wh(.46667rem,.466667rem);
                    svg{
                        @include wh(100%,100%);
                    }
                }
            }
        }
        .myorder:nth-of-type(3) .myorder-div{
            border:0;
        }
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translateX(100%);
    }
</style>
