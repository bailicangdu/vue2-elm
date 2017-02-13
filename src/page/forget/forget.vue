<template>
    <div class="container">
        <head-top head-title="重置密码" goBack="true"></head-top>
        <div>重置密码页</div>    
    </div>
</template>

<script>
    import headTop from '../../components/header/head'

    export default {
        data(){
            return {

            }
        },
        components: {
            headTop,
        },
    }

</script>

<style lang="scss" scoped>
    @import '../../style/mixin.scss';

    .container{
        padding-top: 1.95rem;
    }
</style>
