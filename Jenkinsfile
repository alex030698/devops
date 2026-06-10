pipeline {
    agent any

    stages {
        stage('Checkout') {
            steps {
                checkout scm
            }
        }
        stage('Build') {
            steps {
                dir('bazel_project') {
                    sh 'bazel build //...'
                }
            }
        }
        stage('Test') {
            steps {
                dir('bazel_project') {
                    sh 'bazel test //...'
                }
            }
        }
    }

    post {
        always {
            echo 'Pipeline finished'
        }
        success {
            echo 'SUCCESS'
        }
        failure {
            echo 'FAILED Job'
        }
    }
}