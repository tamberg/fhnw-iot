// SayHelloAlexaSkill, licenced under Apache-2.0

// Based on https://github.com/alexa/skill-sample-nodejs-hello-world

// Intents & Utterances:
//  SayHelloIntent
//      greet me
//      say hi
//      say hello

// Setup:
// $ cd SayHelloAlexaSkill
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

const LaunchRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    handle(handlerInput) {
        const speakOutput = 'Welcome, you can ask me to say Hello.';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const SayHelloIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'SayHelloIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'Hello, friend!';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const HelpIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.HelpIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'You can say hello to me! How can I help?';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const CancelAndStopIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && (Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.CancelIntent'
                || Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.StopIntent');
    },
    handle(handlerInput) {
        const speakOutput = 'Goodbye!';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const SessionEndedRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'SessionEndedRequest';
    },
    handle(handlerInput) {
        // Any cleanup logic goes here.
        return handlerInput.responseBuilder.getResponse();
    }
};

const ErrorHandler = {
    canHandle() {
        return true;
    },
    handle(handlerInput, error) {
        console.log('~~~~ Error handled: ${error.stack}');
        const speakOutput = 'Sorry, I had trouble doing what you asked. Please try again.';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const skillBuilder = Alexa.SkillBuilders.custom()
    .addRequestHandlers(
        LaunchRequestHandler,
        SayHelloIntentHandler,
        HelpIntentHandler,
        CancelAndStopIntentHandler,
        SessionEndedRequestHandler,
    )
    .addErrorHandlers(
        ErrorHandler
    );

const skill = skillBuilder.create();
const adapter = new ExpressAdapter(skill, true, true);

app.post('/', adapter.getRequestHandlers());
app.listen(8080);
