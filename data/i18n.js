class I18n {
  constructor() {
    this.locales = {};
    this.currentLang = localStorage.getItem("fpvgate_lang") || this.detectLanguage();
    this.fallbackLang = "en";
  }

  detectLanguage() {
    const lang = navigator.language || navigator.userLanguage;
    if (lang.startsWith("zh")) {
      return "zh-CN";
    }
    return "en";
  }

  async setLanguage(lang) {
    this.currentLang = lang;
    localStorage.setItem("fpvgate_lang", lang);
    await this.loadLocale(lang);
    this.translatePage();
    document.documentElement.lang = lang;
  }

  async loadLocale(lang) {
    if (this.locales[lang]) return;
    try {
      const response = await fetch(`./locales/${lang}.json`);
      this.locales[lang] = await response.json();
    } catch (error) {
      console.error(`Failed to load locale: ${lang}`, error);
      if (lang !== this.fallbackLang) {
        await this.loadLocale(this.fallbackLang);
      }
    }
  }

  t(key, params = {}) {
    const keys = key.split(".");
    let value = this.locales[this.currentLang] || this.locales[this.fallbackLang];

    for (const k of keys) {
      if (value && value[k]) {
        value = value[k];
      } else {
        value = key;
        break;
      }
    }

    if (typeof value === "string") {
      Object.keys(params).forEach((param) => {
        value = value.replaceAll(`{${param}}`, params[param]);
      });
    }

    return value;
  }

  translatePage() {
    document.querySelectorAll("[data-i18n]").forEach((el) => {
      const key = el.getAttribute("data-i18n");
      const paramsAttr = el.getAttribute("data-i18n-params");
      let params = {};

      if (paramsAttr) {
        try {
          params = JSON.parse(paramsAttr);
        } catch (e) {
          console.error("Failed to parse i18n params:", e);
        }
      }

      const translation = this.t(key, params);
      if (el.tagName === "INPUT" || el.tagName === "TEXTAREA") {
        el.placeholder = translation;
      } else {
        el.innerHTML = translation;
      }
    });

    document.querySelectorAll("[data-i18n-placeholder]").forEach((el) => {
      const key = el.getAttribute("data-i18n-placeholder");
      el.placeholder = this.t(key);
    });

    // Update select options if needed
    document.querySelectorAll("[data-i18n-options]").forEach((el) => {
      const key = el.getAttribute("data-i18n-options");
      const options = this.t(key);
      if (Array.isArray(options)) {
        // This part needs specific implementation based on how options are structured
      }
    });
  }

  async init() {
    await this.loadLocale(this.currentLang);
    this.translatePage();

    // Add language switcher event listener if it exists
    const langSelector = document.getElementById("languageSelector");
    if (langSelector) {
      langSelector.value = this.currentLang;
      langSelector.addEventListener("change", (e) => {
        this.setLanguage(e.target.value);
      });
    }
  }
}

const i18n = new I18n();
document.addEventListener("DOMContentLoaded", () => {
  i18n.init();
});
