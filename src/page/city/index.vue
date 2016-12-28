<template>
  	<div>
        <head-top head-title="上海" go-back='true'>
            <router-link to="/home" slot="changecity" class="change_city">切换城市</router-link>
        </head-top>
        <form class="city_from">
            <div>
                <input type="search" name="city" placeholder="输入学校、商务楼、地址" class="city_input input_style" required>
            </div>
            <div>
                <input type="submit" name="submit" class="city_submit input_style" @click.prevent>
            </div>
        </form>
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
    @import '../../style/mixin';
    
    .change_city{
        right: 0.4rem;
        @include sizecolor(0.6rem, #fff);
        @include center-top;
    }
    .city_from{
        margin-top: 2.35rem;
        background-color: #fff;
        border-top: 1px solid $bordercolor;
        border-bottom: 1px solid $bordercolor;
        padding-top: 0.4rem;
        div{
            width: 90%;
            margin: 0 auto;
            text-align: center;
            .input_style{
                width: 100%;
                height: 1.4rem;
                border-radius: 0.1rem;
                margin-bottom: 0.4rem;
            }
            .city_input{
                border: 1px solid $bordercolor;
                font-size: 0.6rem;
                text-indent: 0.3rem;
            }
            .city_submit{
                background-color: $blue;
                @include sizecolor(0.65rem, #fff);
            }
        }
    }
</style>
