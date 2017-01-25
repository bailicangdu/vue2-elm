<template>
  	<div class="paddingTop">
        <head-top head-title="搜索" goBack="true"></head-top>
        <form class="search_form">
            <input type="search" name="search" placeholder="请输入商家或美食名称" class="search_input" v-model="searchValue" @input="checkInput">
            <input type="submit" name="submit" class="search_submit" @click.prevent="searchTarget">
        </form>
        <section v-if="restaurantList.length">
            <h4 class="title_restaurant">商家</h4>
            <ul class="list_container">
                <router-link :to="{path:'/shop', query:{id:item.id}}" tag="li" v-for="item in restaurantList" :key="item.id" class="list_li">
                    <section class="item_left">
                        <img :src="imgBaseUrl + item.image_path" class="restaurant_img">
                    </section>
                    <section class="item_right">
                        <div class="item_right_text">
                            <p>
                                <span>{{item.name}}</span>
                                <svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="24" height="14" class="pay_icon">
                                    <polygon points="0,14 4,0 24,0 20,14" style="fill:none;stroke:#FF6000;stroke-width:1" />
                                    <line x1="1.5" y1="12" x2="20" y2="12" style="stroke:#FF6000;stroke-width:1.5"/>
                                    <text x="3.5" y="9" style="fill:#FF6000;font-size:9;font-weight:bold;">支付</text>
                                </svg>
                            </p>
                            <p>月售 {{item.month_sales}} 单</p>
                            <p>{{item.delivery_fee}} 元起送 / 距离{{item.distance}}</p>
                        </div>
                        <ul class="item_right_detail">
                            <li v-for="activities in item.restaurant_activity" :key="activities.id">
                                <span :style="{backgroundColor: '#' + activities.icon_color}" class="activities_icon">{{activities.icon_name}}</span>
                                <span>{{activities.name}}</span>
                                <span class="only_phone">(手机客户端专享)</span>
                            </li>
                        </ul>
                    </section>
                </router-link>
            </ul>
        </section>
        <section class="search_history" v-if="searchHistory.length&&showHistory">
            <h4 class="title_restaurant">搜索历史</h4>
            <ul>
                <li v-for="item in searchHistory">{{item}}</li>
            </ul>
        </section>
    </div>
</template>

<script>
import headTop from '../../components/header/head'
import {searchRestaurant} from '../../service/getData'
import {imgBaseUrl} from '../../config/env'
import {getStore, setStore} from '../../config/mUtils'

export default {
	data(){
        return {
            geohash: '', //
            searchValue: '', //
            restaurantList: [], //
            imgBaseUrl, //
            searchHistory: [], //
            showHistory: true, //
        }
    },
    created(){
       
    },
    mounted(){
        this.geohash = this.$route.params.geohash;
        //获取搜索历史记录
        if (getStore('searchHistory')) {
            this.searchHistory = JSON.parse(getStore('searchHistory'));
        }
    },
    components:{
        headTop
    },
    methods:{
        async searchTarget(){
            if (!this.searchValue) {
                return 
            }
            this.showHistory = false;
            this.restaurantList = await searchRestaurant(this.geohash, this.searchValue);
            /**
             * 点击搜索结果进入下一页面时进行判断是否已经有一样的历史记录
             * 如果没有则新增，如果有则不做重复储存，判断完成后进入下一页
             */
            let history = getStore('searchHistory');
            if (history) { 
                let checkrepeat = false;
                this.searchHistory = JSON.parse(history);
                this.searchHistory.forEach(item => {
                    if (item == this.searchValue) {
                        checkrepeat = true;
                    }
                })
                if (!checkrepeat) {
                    this.searchHistory.push(this.searchValue)
                }
            }else {
                this.searchHistory.push(this.searchValue)
            }
            setStore('searchHistory',this.searchHistory)
        },
        checkInput(){
            if (this.searchValue === '') {
                this.showHistory = true;
                this.restaurantList = [];
            } 
        }
    },
    computed:{

    },
}

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    .search_form{
        background-color: #fff;
        padding: 0.5rem;
        .search_input{
            border: 0.025rem solid $bc;
            @include wh(78%, 1.5rem);
            @include sc(0.65rem, #333);
            border-radius: 0.125rem;
            background-color: #f2f2f2;
            font-weight: bold;
            padding: 0 0.25rem;
        }
        .search_submit{
            border: 0.025rem solid $blue;
            @include wh(20%, 1.5rem);
            @include sc(0.65rem, #fff);
            border-radius: 0.125rem;
            background-color: $blue;
            font-weight: bold;
            padding: 0 0.25rem;
        }
    }
    .title_restaurant{
        font-size: 0.7rem;
        line-height: 2rem;
        text-indent: 0.5rem;
        font-weight: bold;
        color: #666;
    }
    .list_container{
        background-color: #fff;
    }
    .list_li{
        display: flex;
        justify-content: 'center';
        padding: 0.5rem;
        border-bottom: 0.025rem solid $bc;
        .item_left{
            margin-right: 0.25rem;
            .restaurant_img{
                @include wh(1.7rem, 1.7rem);
            }
        }
        .item_right{
            font-size: 0.55rem;
            flex: 1;
            .item_right_text{
                padding-bottom: 0.25rem;
                border-bottom: 0.025rem solid $bc;
                p{
                    line-height: .9rem;
                }
                .pay_icon{
                    margin-bottom: -0.08rem;
                }
            }
            .item_right_detail{
                margin-top: 0.25rem;
                li{
                    font-size: 0;
                    span{
                        font-size: .5rem;
                        vertical-align: middle;
                        display: inline-block;
                        margin-bottom: 0.2rem;
                    }
                    .activities_icon{
                        @include sc(.5rem, #fff);
                        font-weight: bold;
                        padding: .04rem;
                        border-radius: 0.15rem;
                        margin-right: 0.125rem;
                    }
                    .only_phone{
                        color: #FF6000;
                    }
                }
            }
        }
    }
</style>
