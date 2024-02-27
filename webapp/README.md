# ESPtemplateApp web frontend

You can run the webapp locally with `npm run dev`. If you enter the IP of your ESP in the `vite.config.ts` beforehand, all api requests will even be proxied to the real ESP. Then you can develop the webapp as if it were running directly on the ESP. The `npm run dev` also supports hot reload, i.e. as soon as you save a vue file, it is automatically reloaded in the browser.

## Project Setup

```sh
npm install
```

### Compile and Hot-Reload for Development

```sh
npm run dev
```

### Type-Check, Compile and Minify for Production

```sh
npm run build
```
