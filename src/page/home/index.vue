<template>
  	<div>
        <head-top signin-up='home'>
            <router-link to='/home' slot='logo' class="head_logo">ele.me</router-link>
        </head-top>
        <nav class="city_nav">
            <div class="city_tip">
                <span>当前定位城市：</span>
                <span>定位不准时，请在城市列表中选择</span>
            </div>
            <router-link :to="'/city/' + guessCityid" class="guess_city">
                <span>{{guessCity}}</span>
                <span>></span>
            </router-link>  
        </nav>
        <div id="hot_city_container">
            <h4 class="ciyt_title">热门城市</h4>
            <ul class="citylistul clear">
                <router-link  tag="li" v-for="item in hotcity" :to="'/city/' + item.id" :key="item.id">
                    {{item.name}}
                </router-link>  
            </ul>
        </div>
        <div class="group_city_container">
            <ul class="letter_classify">
                <li v-for="(value, key, index) in sortgroupciyt" :key="key"  class="letter_classify_li">
                    <h4 class="ciyt_title">{{key}}
                        <span v-if="index == 0">（按字母排序）</span>
                    </h4>
                    <ul class="groupcity_name_container citylistul clear">
                        <router-link  tag="li" v-for="item in value" :to="'/city/' + item.id" :key="item.id" class="ellipsis">
                            {{item.name}}

                        </router-link>  
                    </ul>
                </li>
            </ul>
        </div>
    </div>
</template>

<script>
import headTop from '../../components/header/head'
import {cityGuess, hotcity, groupcity} from '../../service/getData'

export default {
    data(){
        return{
            guessCity: '',
            guessCityid: '',
            hotcity: [],
            groupcity: {},
        }
    },

	created(){
        cityGuess().then(res => {
            this.guessCity = res.name;
            this.guessCityid = res.id;
        })

        hotcity().then(res => {
            this.hotcity = res;
        })

        groupcity().then(res => {
            this.groupcity = res;
        })
    },

    components:{
        headTop
    },

    computed:{
        sortgroupciyt(){
            let sortobj = {};
            for (let i = 65; i <= 90; i++) {
                if (this.groupcity[String.fromCharCode(i)]) {
                    sortobj[String.fromCharCode(i)] = this.groupcity[String.fromCharCode(i)];
                }
            }
            return sortobj
        }
    },

    methods:{

    }
}

</script>

<style lang="scss" scoped>
    @import '../../style/mixin';
    .head_logo{
        left: 0.4rem;
        font-weight: 400;
        @include sizecolor(0.7rem, #fff);
        @include wh(2.3rem, 0.7rem);
        @include center-top;
    }
    .city_nav{
        margin-top: 2.35rem;
        border-top: 1px solid $bordercolor;
        background-color: #fff;
        margin-bottom: 0.4rem;
        .city_tip{
            display: flex;
            justify-content: space-between;
            line-height: 1.45rem;
            padding: 0 0.45rem;
            span:nth-of-type(1){
                @include sizecolor(0.55rem, #666);
            }
            span:nth-of-type(2){
                font-weight: 900;
                @include sizecolor(0.475rem, #9f9f9f);
            }
        }
        .guess_city{
            display: flex;
            height: 1.8rem;
            justify-content: space-between;
            padding: 0 0.45rem;
            border-top: 1px solid $bordercolor;
            border-bottom: 2px solid $bordercolor;
            @include font(0.75rem, 1.8rem);
            span:nth-of-type(1){
                color: $blue;
            }
            span:nth-of-type(2){
                color: #9f9f9f;
            }
        }
    }
    #hot_city_container{
        background-color: #fff;
        margin-bottom: 0.4rem;
    }
    .citylistul{
        li{
            float: left;
            text-align: center;
            color: $blue;
            border-bottom: 0.025rem solid $bordercolor;
            border-right: 0.025rem solid $bordercolor;
            @include wh(25%, 1.75rem);
            @include font(0.6rem, 1.75rem);
        }
        li:nth-of-type(4n){
            border-right: none;
        }
    }
    .ciyt_title{
        color: #666;
        font-weight: 400;
        text-indent: 0.45rem;
        border-top: 2px solid $bordercolor;
        border-bottom: 1px solid $bordercolor;
        @include font(0.55rem, 1.45rem, "Helvetica Neue");
        span{
            @include sizecolor(0.475rem, #999);
        }
    }
    
    .letter_classify_li{
        margin-bottom: 0.4rem;
        background-color: #fff;
        border-bottom: 1px solid $bordercolor;
        .groupcity_name_container{
            li{
                color: #666;
            }
        }
    }

</style>
