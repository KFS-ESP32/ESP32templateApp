<template>
    <BasePage :title="$t('home.LiveData')" :isLoading="dataLoading" :isWideScreen="true" :showWebSocket="true" :lifeCounter = liveData.system.lifeCounter :isWebsocketConnected="isWebsocketConnected" @reload="reloadData">
        <HintView :hints="liveData.hints" />
        <!-- Create 3 colums -->
         <div class="row gy-3">
            <div class="col">{{ $t('home.FirstTODO') }} -- mit Sprachumschaltung!"</div>
            <div class="col">Zweite Spalte -- TODO - -Example-</div>
            <div class="col">Dritte Spalte -- TODO -- -Example-</div>
        </div>
        <!-- Create 2 buttons -->
        <div class="text-center mb-3">
            <button class="btn btn-danger"  type="submit" @click="onClickEventBootStrap" >Button 1  -- TODO -- -Example-</button>
            <button class="btn btn-default" type="submit" @click="onClickEventBootStrap" >Button 2  -- TODO -- -Example-</button>
        </div>

        <!-- Bootstrat Alert -->
        <BootstrapAlert v-model="showAlert" dismissible :variant="alertType"> {{ alertMessage }} </BootstrapAlert>

        <!-- Create a new Card Element -->
        <CardElement :text="$t('home.SetNewText')" textVariant="text-bg-primary" center-content>
            <button class="btn btn-danger" @click="onOpenModal(performModalDialog)">{{ $t('home.SetNewText') }} </button>
            <div class="alert alert-danger mt-3" role="alert" v-html="$t('home.Note')"></div>
        </CardElement>

        <!-- Create a new Card Element -->
        <CardElement :text="$t('home.SetNewText')" textVariant="text-bg-primary" add-space>
             <InputElement :label="('TelegramCounter -Example- :')"
                          v-model="liveData.system.lifeCounter"
                          type="text" disabled/>

            <div class="alert alert-secondary" role="alert" v-html="$t('home.Note')"></div>
        </CardElement>

        <!-- Open Dialog Modal -->
        <ModalDialog modalId="performModalDialog" small :title="$t('home.SetNewText')" :closeText="$t('maintenancereboot.Cancel')"> {{ $t('home.SetNewText') }}
            <template #footer>
                <button type="button" class="btn btn-danger" @click="onClickEvent"> {{ $t('maintenancereboot.Reboot') }}</button>
            </template>
        </ModalDialog>
    </BasePage>
</template>


<script lang="ts">
import BasePage from "@/components/BasePage.vue";
import BootstrapAlert from "@/components/BootstrapAlert.vue";
import CardElement from '@/components/CardElement.vue';
import InputElement from '@/components/InputElement.vue';
import HintView from "@/components/HintView.vue";
import ModalDialog from "@/components/ModalDialog.vue";
import type { LiveData } from '@/types/LiveDataStatus';
import { authHeader, authUrl, handleResponse, isLoggedIn } from "@/utils/authentication";
import * as bootstrap from "bootstrap";
import {} from "bootstrap-icons-vue";
import { defineComponent } from "vue";

export default defineComponent({
  components: {
    BasePage,
    BootstrapAlert,
    CardElement,
    InputElement,
    HintView,
    ModalDialog,
  },
  data() {
    return {
      isLogged: this.isLoggedIn(),
      performModalDialog: {} as bootstrap.Modal,
      socket: {} as WebSocket,
      heartInterval: 0,
      dataLoading: true,
      isFirstFetchAfterConnect: true,

      liveData: {} as LiveData,
      isWebsocketConnected: false,

      // Alert
      alertMessage: " -- ",
      alertType: "info",
      showAlert: false,
    };
  },
  created() {
    this.getInitialData();
    this.initSocket();
    this.$emitter.on("logged-in", () => {
      this.isLogged = this.isLoggedIn();
    });
    this.$emitter.on("logged-out", () => {
      this.isLogged = this.isLoggedIn();
    });
  },
  mounted()
  {
    this.performModalDialog = new bootstrap.Modal('#performModalDialog');
  },
  unmounted() {
    this.closeSocket();
  },
  updated() {
    console.log("Updated");
    // Select first tab
    if (this.isFirstFetchAfterConnect) {
      console.log("isFirstFetchAfterConnect");

      this.$nextTick(() => {
        console.log("nextTick");
        const firstTabEl = document.querySelector(
          "#v-pills-tab:first-child button"
        );
        if (firstTabEl != null) {
          this.isFirstFetchAfterConnect = false;
          console.log("Show");
          const firstTab = new bootstrap.Tab(firstTabEl);
          firstTab.show();
        }
      });
    }
  },
  methods: {
   isLoggedIn,
    onOpenModal(modal: bootstrap.Modal) {
        modal.show();
    },
    onCloseModal(modal: bootstrap.Modal) {
        modal.hide();
    },
    onClickEvent() {
        this.onCloseModal(this.performModalDialog);
    },
    onClickEventBootStrap() {
        this.alertMessage =  "-- BootstrapAlert --";
        this.alertType = 'danger';
        this.showAlert = true;
    },

    getInitialData(triggerLoading: boolean = true) {
      if (triggerLoading) {
        this.dataLoading = true;
      }
      fetch("/api/livedata/status", { headers: authHeader() })
        .then((response) =>
          handleResponse(response, this.$emitter, this.$router)
        )
        .then((data) => {
          this.liveData = data;
          if (triggerLoading) {
            this.dataLoading = false;
          }
        });
    },
    reloadData() {
      this.closeSocket();

      setTimeout(() => {
        this.getInitialData(false);
        this.initSocket();
      }, 1000);
    },
    initSocket() {
      console.log("Starting connection to WebSocket Server");

      const { protocol, host } = location;
      const authString = authUrl();
      const webSocketUrl = `${ protocol === "https:" ? "wss" : "ws" }://${authString}${host}/livedata`;
      this.socket = new WebSocket(webSocketUrl);

      this.socket.onmessage = (event) => {
        console.log(event);
        if (event.data != "{}") {
          this.liveData = JSON.parse(event.data);
          this.dataLoading = false;
          this.heartCheck(); // Reset heartbeat detection
        } else {
          // Sometimes it does not recover automatically so have to force a reconnect
          this.closeSocket();
          this.heartCheck(10); // Reconnect faster
        }
      };


      this.socket.onopen = (event) => {
        console.log(event);
        console.log("Successfully connected to the echo websocket server...");
        this.isWebsocketConnected = true;
      };

      this.socket.onclose = () => {
        console.log("Connection to websocket closed...");
        this.isWebsocketConnected = false;
      };

      // Listen to window events , When the window closes , Take the initiative to disconnect websocket Connect
      window.onbeforeunload = () => {
        this.closeSocket();
      };
    },
    // Send heartbeat packets regularly * 59s Send a heartbeat
    heartCheck(duration: number = 59) {
      this.heartInterval && clearTimeout(this.heartInterval);
      this.heartInterval = setInterval(() => {
        if (this.socket.readyState === 1) {
          // Connection status
          this.socket.send("ping");
        } else {
          this.initSocket(); // Breakpoint reconnection 5 Time
        }
      }, duration * 1000);
    },
    /** To break off websocket Connect */
    closeSocket() {
      this.socket.close();
      this.heartInterval && clearTimeout(this.heartInterval);
      this.isFirstFetchAfterConnect = true;
    },
  },
});
</script>

<style>
.btn-group {
  border-radius: var(--bs-border-radius);
  margin-top: 0.25rem;
}
</style>
