<template>
    <div>
        <head-top head-title="确认订单" goBack="true"></head-top>
        <div class="confirmOrderContainer">确认订单页</div>    
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

    .confirmOrderContainer{
        padding-top: 1.95rem;
    }
</style>
