<template>
    <BasePage :title="$t('about.AboutAPP')">
        <div class="accordion" id="accordionExample">
            <div class="accordion-item">
                <h2 class="accordion-header" id="headingOne">
                    <button class="accordion-button" type="button" data-bs-toggle="collapse"
                        data-bs-target="#collapseOne" aria-expanded="true" aria-controls="collapseOne">
                        <span class="badge text-bg-secondary">
                            <BIconInfoCircle class="fs-4" />
                        </span>&nbsp;{{ $t('about.ProjectOrigin') }}
                    </button>
                </h2>
                <div id="collapseOne" class="accordion-collapse collapse show" aria-labelledby="headingOne"
                    data-bs-parent="#accordionExample">
                    <div class="accordion-body">
                        <p class="fw-normal" v-html="$t('about.ProjectOriginBody1')"></p>
                        <p class="fw-normal" v-html="$t('about.ProjectOriginBody2')"></p>
                    </div>
                </div>
            </div>
            <div class="accordion-item">
                <h2 class="accordion-header" id="headingTwo">
                    <button class="accordion-button collapsed" type="button" data-bs-toggle="collapse"
                        data-bs-target="#collapseTwo" aria-expanded="false" aria-controls="collapseTwo">
                        <span class="badge text-bg-secondary">
                            <BIconActivity class="fs-4" />
                        </span>&nbsp;{{ $t('about.NewsUpdates') }}
                    </button>
                </h2>
                <div id="collapseTwo" class="accordion-collapse collapse" aria-labelledby="headingTwo"
                    data-bs-parent="#accordionExample">
                    <div class="accordion-body" v-html="$t('about.NewsUpdatesBody')"></div>
                </div>
            </div>
        </div>
    </BasePage>
</template>

<script lang="ts">
import BasePage from '@/components/BasePage.vue';
import { BIconActivity, BIconInfoCircle } from 'bootstrap-icons-vue';
import { defineComponent } from 'vue';

export default defineComponent({
    components: {
        BasePage,
        BIconActivity,
        BIconInfoCircle,
    },
});

</script>
