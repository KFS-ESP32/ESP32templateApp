<template>
    <NavBar :title="$t('home.AppName')"/>
    <router-view />
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import NavBar from './components/NavBar.vue';

export default defineComponent({
    name: 'App',
    components: {
        NavBar,
    },
});
</script>

<style>
/* Show it is fixed to the top */
body {
    min-height: 75rem;
    padding-top: 4.5rem;
}
</style>
