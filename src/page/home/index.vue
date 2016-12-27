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
        </nav>
    </div>
</template>

<script>
import headTop from '../../components/header/head'
export default {
    data(){
        return{

        }
    },

	created(){
        
    },

    components:{
        headTop
    },

    computed:{

    },

    methods:{

    }
}

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';
    .head_logo{
        position: absolute;
        width: 2.3rem;
        height: 0.7rem;
        left: 0.4rem;
        top: 50%;
        transform: translateY(-50%);
        font-size: 0.7rem;
        color: #fff;
        font-weight: 400;
    }
    .city_nav{
        margin-top: 2.35rem;
        border-top: 2px solid $bordercolor;
        background-color: #fff;
        .city_tip{
            color: #666;
        }
    }
</style>
