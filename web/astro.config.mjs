import { defineConfig } from 'astro/config';

import tailwind from "@astrojs/tailwind";

// https://astro.build/config
export default defineConfig({
  site: 'https://engineering.purdue.edu',
  base: '/477grp5',
  integrations: [tailwind()]
});