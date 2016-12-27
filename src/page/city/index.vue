<template>
  	<div>
        <head-top head-title="上海" go-back='true'>
            <router-link to="/home" slot="changecity" class="change_city">切换城市</router-link>
        </head-top>
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
</style>
