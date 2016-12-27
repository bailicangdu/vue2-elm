<template>
    <header id='head_top'>
        <slot name='logo'></slot>
        <router-link to='/login' v-if='signinUp' class="head_login">登陆|注册</router-link>
    </header>
</template>

<script>

    export default {
    	data(){
            return{
                
            }
        },
        created(){

        },
        mounted(){

        },
        props: ['signinUp'],
        computed: {
            
        },
        methods: {

        },

    }

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    #head_top{
        height: 1.95rem;
        width: 100%;
        background-color: $blue;
        position: fixed;
        left: 0;
        top: 0;
    }
    .head_login{
        position: absolute;
        font-size: 0.6rem;
        color: #fff;
        right: 0.55rem;
        top: 50%;
        transform: translateY(-50%);
    }
</style>
