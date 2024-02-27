<template>
    <BasePage :title="$t('systeminfo.SystemInfo')" :isLoading="dataLoading" :show-reload="true" @reload="getSystemInfo">
        <FirmwareInfo :systemStatus="systemDataList" v-model:allowVersionInfo="allowVersionInfo" />
        <div class="mt-5"></div>
        <HardwareInfo :systemStatus="systemDataList" />
        <div class="mt-5"></div>
        <MemoryInfo :systemStatus="systemDataList" />
        <div class="mt-5"></div>
        <HeapDetails :systemStatus="systemDataList" />
        <div class="mt-5"></div>
    </BasePage>
</template>

<script lang="ts">
import BasePage from '@/components/BasePage.vue';
import FirmwareInfo from "@/components/FirmwareInfo.vue";
import HardwareInfo from "@/components/HardwareInfo.vue";
import MemoryInfo from "@/components/MemoryInfo.vue";
import HeapDetails from "@/components/HeapDetails.vue";
import type { SystemStatus } from '@/types/SystemStatus';
import { authHeader, handleResponse } from '@/utils/authentication';
import { defineComponent } from 'vue';

export default defineComponent({
    components: {
        BasePage,
        FirmwareInfo,
        HardwareInfo,
        MemoryInfo,
        HeapDetails,
    },
    data() {
        return {
            dataLoading: true,
            systemDataList: {} as SystemStatus,
            allowVersionInfo: false,
        }
    },
    created() {
        this.getSystemInfo();
    },
    methods: {
        getSystemInfo() {
            this.dataLoading = true;
            fetch("/api/system/status", { headers: authHeader() })
                .then((response) => handleResponse(response, this.$emitter, this.$router))
                .then((data) => {
                    this.systemDataList = data;
                    this.dataLoading = false;
                })
        },
    },
    watch: {

    }
});
</script>
