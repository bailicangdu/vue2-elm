<template>
    <div class="profile_page">
        <head-top go-back='true' :head-title="profiletitle"></head-top>
        <section class="profile-number">
            <router-link :to="getUserinfo? '/profile/info' : '/login'" class="profile-link">
                <img :src="imgpath" class="privateImage" v-if="this.avatar">
                <span class="privateImage" v-else>
                    <svg class="privateImage-svg">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#avatar-default"></use>
                    </svg>
                </span>
                <div class="user-info">
                    <p>{{username}}</p>
                    <p>
                        <span class="user-icon">
                            <svg class="icon-mobile" fill="#fff">
                                <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#mobile"></use>
                            </svg>
                        </span>
                        <span class="icon-mobile-number">{{mobile}}</span>
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
                    <span class="info-data-top"><b>{{balance}}</b>元</span>
                    <span class="info-data-bottom">我的余额</span>
                </router-link>
                <router-link to="/profile/benefit" tag="li" class="info-data-link">
                    <span class="info-data-top"><b>{{count}}</b>个</span>
                    <span class="info-data-bottom">我的优惠</span>
                </router-link>
                <router-link to="/profile/points" tag="li" class="info-data-link">
                    <span class="info-data-top"><b>{{pointNumber}}</b>分</span>
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
            <router-link to='/service' class="myorder">
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
            <router-link to='/download' class="myorder">
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
        <foot-guide></foot-guide>
        <transition name="router-slid">
            <router-view></router-view>
        </transition>
    </div>
</template>

<script>
import headTop from '../../components/header/head'
import footGuide from '../../components/footer/footGuide'
import {mapState, mapMutations} from 'vuex'
import {imgBaseUrl} from '../../config/env'
import {getImgPath} from '../../components/common/mixin'

export default {
    data(){
        return{
            profiletitle: '我的',
            getUserinfo: {},        //得到数据 
            username: '登陆/注册',           //用户名
            resetname: '',
            mobile: '登陆后享受更多特权',             //电话号码
            balance: '0',            //我的余额
            count : '0',             //优惠券个数
            pointNumber : '0',       //积分数
            avatar: '',             //头像地址
            imgBaseUrl,
        }
    },

    mounted(){
        this.getUserinfo = this.userInfo;
        if (this.userInfo) {
            this.avatar = this.getUserinfo.avatar;
            //this.resetname=this.userInfo.resetname; 
            this.username =this.getUserinfo.username;
            this.mobile = this.getUserinfo.mobile;
            this.balance = this.getUserinfo.balance;
            this.count = this.getUserinfo.gift_amount;
            this.pointNumber = this.getUserinfo.point;
        }
    },

    mixins: [getImgPath],

    components:{
        headTop,
        footGuide,
    },

    computed:{
        ...mapState([
            'userInfo', 
        ]),
        imgpath:function () {
            let path;
            if(this.avatar.indexOf('/') !==-1){
                path = imgBaseUrl +　this.avatar;
            }else{
                path = this.getImgPath(this.avatar)
            }
            this.SAVE_AVANDER(path);
            return path;
        }
    },

    methods:{
        ...mapMutations([
            'SAVE_AVANDER'
        ]),
    },
    watch: {
        userInfo: function (){
            this.getUserinfo = this.userInfo;
            this.avatar = this.getUserinfo&&this.getUserinfo.avatar || '';
            this.username = this.getUserinfo&&this.getUserinfo.username ||'登陆/注册';
            this.mobile = this.getUserinfo&&this.getUserinfo.mobile ||'登陆后享受更多特权';
            this.balance = this.getUserinfo&&this.getUserinfo.balance || '0';
            this.count = this.getUserinfo&&this.getUserinfo.gift_amount || '0';
            this.pointNumber = this.getUserinfo&&this.getUserinfo.point || '0';
        }
    }
}

</script>

<style lang="scss" scoped>
   @import '../../style/mixin'; 
    
    .profile_page{
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
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
