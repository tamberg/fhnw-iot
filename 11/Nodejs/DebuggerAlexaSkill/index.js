// DebuggerAlexaSkill, licenced under Apache-2.0

// Based on https://github.com/alexa/skill-sample-nodejs-hello-world

// Setup:
// $ cd DebuggerAlexaSkill
// $ npm install ask-sdk-core
// $ npm install ask-sdk-model
// $ npm install ask-sdk-express-adapter
// $ npm install express

// Run:
// $ node index.js

const Alexa = require('ask-sdk-core');
const express = require('express');
const { ExpressAdapter } = require('ask-sdk-express-adapter');

const app = express();

const IntentReflectorHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest';
    },
    handle(handlerInput) {
        const intentName = Alexa.getIntentName(handlerInput.requestEnvelope);
        const speakOutput = `You just triggered ${intentName}`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const ErrorHandler = {
    canHandle() {
        return true;
    },
    handle(handlerInput, error) {
        console.log('~~~~ Error handled: ${error.stack}');
        const speakOutput = 'Oops, there was an error.';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const skillBuilder = Alexa.SkillBuilders.custom()
    .addRequestHandlers(IntentReflectorHandler)
    .addErrorHandlers(ErrorHandler);

const skill = skillBuilder.create();
const adapter = new ExpressAdapter(skill, true, true);

app.post('/', adapter.getRequestHandlers());
app.listen(8080);