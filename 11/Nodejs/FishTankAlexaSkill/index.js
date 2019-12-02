/* eslint-disable  func-names */
/* eslint quote-props: ["error", "consistent"]*/

'use strict';

const Alexa = require('alexa-sdk');

const APP_ID = undefined;  // TODO replace with your app ID (OPTIONAL).

const languageStrings = {
    'en': {
        translation: {
            SKILL_NAME: 'Fish Tank',
            GET_FED_TODAY_MESSAGE: 'Yes, you fed your fish today.',
            GET_FED_AT_DATE_MESSAGE: 'You last fed your fish 2 days ago.'
        },
    },
    'de': {
        translation: {
            SKILL_NAME: 'Fish Tank auf Deutsch',
            GET_FED_TODAY_MESSAGE: 'Ja, du hast deine Fische heute gefüttert.',
            GET_FED_AT_DATE_MESSAGE: 'Du hast deine Fische letztmals vor 2 Tagen gefüttert.'
        },
    },
};

const handlers = {
    'GetLastFeeding': function () {
        console.log('GetLastFeeding');
        this.emit(':tell', this.t('GET_FED_TODAY_MESSAGE'));
    },
    'GetFedToday': function () {
        console.log('GetFedToday');
        this.emit(':tell', this.t('GET_FED_TODAY_MESSAGE'));
    },
    'GetFedAtDate': function (date) {
        console.log('GetFedAtDate');
        this.emit(':tell', this.t('GET_FED_AT_DATE_MESSAGE'));
    },
};

exports.handler = function (event, context) {
    const alexa = Alexa.handler(event, context);
    alexa.APP_ID = APP_ID;
    // To enable string internationalization (i18n) features, set a resources object.
    alexa.resources = languageStrings;
    alexa.registerHandlers(handlers);
    alexa.execute();
};

// package.json

{
  "name": "alexa-skill-kit-sdk-factskill",
  "version": "1.0.0",
  "private": true,
  "dependencies": {
    "alexa-sdk": "^1.0.10"
  }
}

