<template>
  	<div class="marginTop">
        <head-top head-title="搜索" goBack="true"></head-top>
        <form class="search_form">
            <input type="search" name="search" class="search_input">
            <input type="submit" name="submit" class="search_submit">
        </form>
    </div>
</template>

<script>
import headTop from '../../components/header/head'


export default {
	data(){
        return {

        }
    },
    mounted(){

    },
    components:{
        headTop
    },
    methods:{

    },
    computed:{

    },

}

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    .search_form{
        background-color: #fff;
        .search_input{

        }
        .search_submit{
            
        }
    }

</style>
