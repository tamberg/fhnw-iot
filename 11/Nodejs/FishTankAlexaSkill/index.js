// SayHelloAlexaSkill, licenced under Apache-2.0

// Based on https://github.com/alexa/skill-sample-nodejs-hello-world

// See also https://developer.amazon.com/alexa/console/ask

// Intents & Utterances:
//  GetLastFeeding
//      when I gave food to the fish
//      when I last fed the fish
//  GetFedToday
//      did I give the fish any food
//      if I fed the fish
//  GetFedAtDate
//      did I feed the fish {Date}
//      if I fed the fish {Date}
//      Date Slot: AMAZON.DATE

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

const GetLastFeedingIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'GetLastFeedingIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'You did not feed them yet, at all.';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const GetFedTodayIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'GetFedTodayIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'You did not feed them yet, today.';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const GetFedAtDateIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'GetFedAtDateIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'You did not feed them yet, on date.';
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
        console.log(`~~~~ Error handled: ${error.stack}`);
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
        GetLastFeedingIntentHandler,
        GetFedTodayIntentHandler,
        GetFedAtDateIntentHandler,
        HelpIntentHandler,
        CancelAndStopIntentHandler,
        SessionEndedRequestHandler
    )
    .addErrorHandlers(
        ErrorHandler
    );

const skill = skillBuilder.create();
const adapter = new ExpressAdapter(skill, true, true);

app.post('/', adapter.getRequestHandlers());
app.listen(8080);
